/**
 * kernel/shell_input.c - Advanced shell input implementation
 * Features:
 * - Command history (UP/DOWN to navigate)
 * - Cursor movement (LEFT/RIGHT, HOME, END)
 * - Backspace, Delete keys
 * - Auto-scroll when reaching bottom of screen
 * - Ctrl+L for clear, Ctrl+U for clear line
 */

#include "shell_input.h"
#include "vga.h"
#include "kbd.h"
#include "string.h"
#include "printk.h"

void shell_input_init(shell_input_t *input) {
    input->cursor_pos = 0;
    input->buffer_len = 0;
    input->screen_row = 0;
    input->history_index = -1;
    input->history_count = 0;
    memset(input->buffer, 0, SHELL_INPUT_MAX_LEN);
}

void shell_input_clear(shell_input_t *input) {
    input->cursor_pos = 0;
    input->buffer_len = 0;
    input->history_index = -1;
    memset(input->buffer, 0, SHELL_INPUT_MAX_LEN);
}

void shell_input_add_history(shell_input_t *input, const char *cmd) {
    if (input->history_count >= SHELL_INPUT_HISTORY) {
        /* Shift history up (FIFO) */
        for (int i = 0; i < SHELL_INPUT_HISTORY - 1; i++) {
            strcpy(input->history[i], input->history[i+1]);
        }
        input->history_count--;
    }
    strcpy(input->history[input->history_count], cmd);
    input->history_count++;
    input->history_index = -1; /* Reset to "new command" mode */
}

int shell_input_read_key(void) {
    static int extended = 0;
    
    unsigned char scancode = kbd_read_scancode();
    if (scancode == 0) {
        return 0;  /* No key available */
    }
    
    /* Check if it's an extended key prefix (0xE0) */
    if (scancode == 0xE0) {
        extended = 1;
        return 0;  /* Wait for next scancode, which we'll get on next call */
    }
    
    /* Ignore key releases (high bit set) */
    if (scancode & 0x80) {
        if (extended) {
            extended = 0;
        }
        return 0;
    }
    
    /* Convert extended scancodes to special key codes */
    if (extended) {
        extended = 0;
        switch (scancode) {
            case 0x48: return KEY_UP;
            case 0x50: return KEY_DOWN;
            case 0x4B: return KEY_LEFT;
            case 0x4D: return KEY_RIGHT;
            case 0x47: return KEY_HOME;
            case 0x4F: return KEY_END;
            case 0x49: return KEY_PAGE_UP;
            case 0x51: return KEY_PAGE_DOWN;
            case 0x52: return KEY_INSERT;
            case 0x53: return KEY_DELETE;
            default: return 0;
        }
    }
    
    /* Convert regular scancode to ASCII or special key code */
    return scancode_to_ascii(scancode);
}

static void redraw_line(shell_input_t *input, const char *prompt) {
    /* Save cursor position */
    int saved_row = vga_get_cursor_row();
    int prompt_len = strlen(prompt);
    
    /* Move to start of line and clear it */
    vga_set_cursor(saved_row, 0);
    for (int i = 0; i < 80; i++) {
        vga_putc(' ');
    }
    
    /* Redraw prompt and input */
    vga_set_cursor(saved_row, 0);
    vga_write(prompt, prompt_len);
    
    /* Display buffer starting from position that fits on screen */
    int display_start = 0;
    int available = 80 - prompt_len - 1;
    
    if (input->buffer_len > available) {
        display_start = input->buffer_len - available;
    }
    
    for (int i = display_start; i < input->buffer_len; i++) {
        vga_putc(input->buffer[i]);
    }
    
    /* Position cursor correctly */
    int cursor_col = prompt_len + (input->cursor_pos - display_start);
    vga_set_cursor(saved_row, cursor_col);
}

char* shell_input_read_line(shell_input_t *input, const char *prompt) {
    shell_input_clear(input);
    input->screen_row = vga_get_cursor_row();
    
    vga_write(prompt, strlen(prompt));
    
    while (1) {
        int key = shell_input_read_key();
        
        /* No key available - try again */
        if (key == 0) continue;
        
        /* Printable ASCII character */
        if (key >= 32 && key < 127) {
            if (input->buffer_len >= SHELL_INPUT_MAX_LEN - 1) {
                continue; /* Buffer full */
            }
            
            /* Insert character at cursor position */
            for (int i = input->buffer_len; i > input->cursor_pos; i--) {
                input->buffer[i] = input->buffer[i-1];
            }
            input->buffer[input->cursor_pos] = (char)key;
            input->cursor_pos++;
            input->buffer_len++;
            input->history_index = -1; /* Exit history mode */
            
            vga_putc((char)key);
            redraw_line(input, prompt);
        }
        /* Enter key - command ready */
        else if (key == '\n') {
            input->buffer[input->buffer_len] = '\0';
            vga_putc('\n');
            
            if (input->buffer_len > 0) {
                shell_input_add_history(input, input->buffer);
            }
            
            /* Auto-scroll if at bottom */
            int current_row = vga_get_cursor_row();
            if (current_row >= 24) {
                scroll_screen();
            }
            
            return input->buffer;
        }
        /* Backspace */
        else if (key == '\b') {
            if (input->cursor_pos > 0) {
                input->cursor_pos--;
                for (int i = input->cursor_pos; i < input->buffer_len - 1; i++) {
                    input->buffer[i] = input->buffer[i+1];
                }
                input->buffer_len--;
                input->history_index = -1;
                redraw_line(input, prompt);
            }
        }
        /* Delete key */
        else if (key == KEY_DELETE) {
            if (input->cursor_pos < input->buffer_len) {
                for (int i = input->cursor_pos; i < input->buffer_len - 1; i++) {
                    input->buffer[i] = input->buffer[i+1];
                }
                input->buffer_len--;
                redraw_line(input, prompt);
            }
        }
        /* Left arrow */
        else if (key == KEY_LEFT) {
            if (input->cursor_pos > 0) {
                input->cursor_pos--;
                redraw_line(input, prompt);
            }
        }
        /* Right arrow */
        else if (key == KEY_RIGHT) {
            if (input->cursor_pos < input->buffer_len) {
                input->cursor_pos++;
                redraw_line(input, prompt);
            }
        }
        /* Home key */
        else if (key == KEY_HOME) {
            input->cursor_pos = 0;
            redraw_line(input, prompt);
        }
        /* End key */
        else if (key == KEY_END) {
            input->cursor_pos = input->buffer_len;
            redraw_line(input, prompt);
        }
        /* Up arrow - previous history */
        else if (key == KEY_UP) {
            if (input->history_count > 0) {
                if (input->history_index < 0) {
                    input->history_index = input->history_count - 1;
                } else if (input->history_index > 0) {
                    input->history_index--;
                }
                strcpy(input->buffer, input->history[input->history_index]);
                input->buffer_len = strlen(input->buffer);
                input->cursor_pos = input->buffer_len;
                redraw_line(input, prompt);
            }
        }
        /* Down arrow - next history */
        else if (key == KEY_DOWN) {
            if (input->history_count > 0 && input->history_index >= 0) {
                input->history_index++;
                if (input->history_index >= input->history_count) {
                    shell_input_clear(input);
                    input->history_index = -1;
                } else {
                    strcpy(input->buffer, input->history[input->history_index]);
                    input->buffer_len = strlen(input->buffer);
                    input->cursor_pos = input->buffer_len;
                }
                redraw_line(input, prompt);
            }
        }
        /* Ctrl+L - clear screen */
        else if (key == CTRL_L) {
            vga_clear_screen();
            input->screen_row = 0;
            vga_set_cursor(0, 0);
            vga_write(prompt, strlen(prompt));
            int prompt_len = strlen((char *)prompt);
            for (int i = 0; i < input->buffer_len; i++) {
                vga_putc(input->buffer[i]);
            }
            vga_set_cursor(0, prompt_len + input->cursor_pos);
        }
        /* Ctrl+U - clear line */
        else if (key == CTRL_U) {
            shell_input_clear(input);
            input->history_index = -1;
            redraw_line(input, prompt);
        }
    }
    
    return NULL;
}
