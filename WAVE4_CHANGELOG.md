# Wave 4: Advanced Keyboard & Shell Input System

## Overview
This wave implements a complete keyboard input system with command history, full arrow key navigation, and auto-scrolling when reaching screen bottom.

## Files Created

### `include/shell_input.h`
- Advanced shell input module with history management
- Supports UP/DOWN for history navigation
- Supports LEFT/RIGHT, HOME, END for cursor movement
- Supports DELETE, Backspace for editing
- Auto-scroll when reaching screen bottom
- Ctrl+L for clear screen, Ctrl+U for clear line
- Defines KEY_* constants for special keys

### `kernel/shell_input.c`
- Complete implementation of shell input with:
  - Command history (FIFO buffer, 10 entries)
  - Cursor positioning (horizontal and vertical)
  - Real-time line redrawing during editing
  - Extended key detection (0xE0 prefix handling)
  - Auto-scroll on Enter when at cursor row >= 24
  - Helper functions for history and buffer management

## Files Modified

### `drivers/kbd.c`
- Added CTRL key detection (0x1D pressed, 0x9D released)
- Added ALT key detection (0x38 pressed, 0xB8 released)
- Added NUM LOCK toggle (0x45)
- Extended special key handling for Ctrl combinations:
  - Ctrl+C → 3 (interrupt)
  - Ctrl+D → 4 (EOF)
  - Ctrl+L → 12 (clear screen)
  - Ctrl+U → 21 (clear line)
  - Ctrl+W → 23 (kill word)
- Improved key release detection

### `include/vga.h`
- Added new function declarations:
  - `vga_putc()` — put single character
  - `vga_get_cursor_row()` — get current row (0-24)
  - `vga_get_cursor_col()` — get current column (0-79)
  - `vga_set_cursor()` — set cursor to row/col
  - `vga_clear_screen()` — alias for clear_screen()

### `drivers/vga.c`
- Implemented cursor position getter/setter functions
- Added vga_putc() for character output
- Added vga_clear_screen() alias
- Bounds checking on cursor positioning

### `kernel/kernel.c`
- Added `#include "shell_input.h"`
- Completely rewrote `shell_loop()`:
  - Old: Manual scancode polling with simple buffer
  - New: Uses shell_input module with history & navigation
  - Calls shell_input_read_line() which handles all input
  - Each line automatically saved to history
- Kept handle_command() unchanged (backward compatible)

### `Makefile`
- Added `kernel/shell_input.o` to OBJ compilation list

## Features Implemented

✅ Command History (UP/DOWN arrows)
- Browse last 10 commands
- Return to "new command" mode when pressing DOWN beyond history
- History automatically saved on Enter

✅ Cursor Navigation
- LEFT/RIGHT arrows move cursor within current line
- HOME key moves to start of line
- END key moves to end of line
- Cursor position synced with buffer

✅ Line Editing
- Backspace deletes character before cursor
- Delete (0x53) deletes character at cursor
- Text shifts left/right on insertion/deletion
- Line redraws in place during editing

✅ Extended Keyboard
- All AZERTY characters (lowercase, uppercase, symbols)
- Special keys: Tab, Enter, Backspace, Escape
- Modifiers: Shift, Caps Lock, Ctrl, Alt, Num Lock
- Numpad: +, -, *

✅ Auto-Scroll
- When reaching row 24, scroll_screen() activates
- Screen shifts up by one line
- User can continue typing at bottom

✅ Ctrl Combinations
- Ctrl+L clears screen and redraws prompt
- Ctrl+U clears current input line
- Ctrl+C/D reserved for signal handling

## Testing Checklist

- [ ] Boot OS in VirtualBox
- [ ] Type commands and verify they execute
- [ ] Press UP arrow, verify last command reappears
- [ ] Press UP again, verify second-to-last command
- [ ] Press DOWN, verify new command mode restored
- [ ] Type partial text, press HOME, verify cursor at start
- [ ] Press END, verify cursor at end
- [ ] Type text, use LEFT/RIGHT to edit middle of line
- [ ] Press DELETE key to remove characters
- [ ] Type many lines to test auto-scroll at row 24
- [ ] Press Ctrl+L to clear and verify screen clears properly
- [ ] Test Ctrl+U to clear input line
- [ ] Verify all AZERTY keys work (including symbols with Shift)
- [ ] Test Numpad keys (+, -, *)

## Known Limitations

1. Extended keyboard (arrow keys) requires 0xE0 scancode prefix
2. Some BIOS emulation might not send all extended codes properly
3. F1-F12 keys not yet implemented (reserved for future)
4. Copy/Paste not supported
5. History limited to 10 entries (configurable via SHELL_INPUT_HISTORY)

## Performance Impact

- Minimal: All operations O(n) on input buffer length (max 256 chars)
- History lookup O(1)
- No additional memory beyond buffer + history + state struct
- ~1KB total additional memory per shell instance

## Next Steps (Wave 5 Ideas)

- [ ] Tab completion for command names
- [ ] Search command history (Ctrl+R)
- [ ] Syntax highlighting in shell
- [ ] Multi-line command continuation (\)
- [ ] Command aliasing
- [ ] Custom key bindings

---

**Status**: Ready for testing
**Build**: `make clean && make` (adds shell_input.o)
**Test**: Boot myos.iso in VirtualBox, interact with shell
