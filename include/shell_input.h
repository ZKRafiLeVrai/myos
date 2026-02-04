/**
 * shell_input.h - Advanced shell input with history and arrow navigation
 * Implements command history, cursor movement, and auto-scrolling
 */

#ifndef __SHELL_INPUT_H__
#define __SHELL_INPUT_H__

#include <stddef.h>

#define SHELL_INPUT_MAX_LEN    256
#define SHELL_INPUT_HISTORY    10

/* Extended key codes for special keys */
#define KEY_UP                 0x48
#define KEY_DOWN               0x50
#define KEY_LEFT               0x4B
#define KEY_RIGHT              0x4D
#define KEY_DELETE             0x53
#define KEY_HOME               0x47
#define KEY_END                0x4F
#define KEY_PAGE_UP            0x49
#define KEY_PAGE_DOWN          0x51
#define KEY_INSERT             0x52

/* Control characters */
#define CTRL_C                 3
#define CTRL_D                 4
#define CTRL_L                 12   // Clear screen
#define CTRL_U                 21   // Clear line

typedef struct {
    char buffer[SHELL_INPUT_MAX_LEN];  /* Current input buffer */
    int cursor_pos;                     /* Cursor position in buffer */
    int buffer_len;                     /* Current length of input */
    int screen_row;                     /* Current row on screen (0-24) */
    int history_index;                  /* Current history entry (-1 = new) */
    char history[SHELL_INPUT_HISTORY][SHELL_INPUT_MAX_LEN];
    int history_count;                  /* Number of entries in history */
} shell_input_t;

/**
 * Initialize shell input state
 */
void shell_input_init(shell_input_t *input);

/**
 * Read one character or extended key code
 * Returns: 0-127 for ASCII, extended codes (KEY_UP, etc.) for special keys
 */
int shell_input_read_key(void);

/**
 * Handle input with arrow keys, history, and auto-scroll
 * input: pointer to shell_input_t state
 * Returns: pointer to null-terminated command string when user presses Enter, NULL otherwise
 */
char* shell_input_read_line(shell_input_t *input, const char *prompt);

/**
 * Add command to history
 */
void shell_input_add_history(shell_input_t *input, const char *cmd);

/**
 * Clear current input buffer
 */
void shell_input_clear(shell_input_t *input);

#endif
