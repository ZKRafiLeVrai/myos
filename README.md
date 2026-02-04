MyOS - 32-bit Hobby Operating System

A minimal but feature-rich 32-bit protected mode kernel for learning OS development.

## Features

✅ **Protected Mode Boot** — 32-bit GDT/IDT setup with exception handling  
✅ **Memory Management** — Kernel heap allocator (kmalloc) with alignment support  
✅ **File System** — Simple RAM-based VFS with file create/read/delete  
✅ **Keyboard Driver** — AZERTY layout with full key mapping  
✅ **Video Driver** — VGA text mode with color support and scrolling  
✅ **PCI Bus Scanning** — Device discovery and logging  
✅ **RTC/Timer** — Programmable interval timer with tick counter  
✅ **Shell** — Interactive command-line interface with ~12 commands  
✅ **Kernel Logging** — Categorized log levels (INFO, WARN, ERROR)  
✅ **Statistics** — Real-time heap usage, uptime, device tracking  
✅ **Safety Features** — Assertions, bounds checking, error codes  

---

## Quick Start (CI-Based Build)

### 1. Build on GitHub (No Local Toolchain Needed)

Push this repo to GitHub, then:
- Go to **Actions** tab → **Build MyOS (i386)**
- Download `myos-bin` or `myos-iso` artifact from the latest successful run

### 2. Test Locally with QEMU

```bash
# Install QEMU (Windows/macOS/Linux)
qemu-system-i386 -m 512 -kernel myos.bin
# or with ISO:
qemu-system-i386 -m 512 -cdrom myos.iso
```

### 3. VirtualBox (as shown)

- Create new VM (Linux 2.6+, 512 MB RAM)
- Attach myos.iso as CD-ROM boot media
- Boot and enjoy!

---

## Available Commands

| Command | Description |
|---------|-------------|
| `ls` | List files in root |
| `cat` | Read motd.txt |
| `touch` | Create nouveau.txt |
| `stats` | Show kernel statistics |
| `dmesg` | Kernel log buffer |
| `date` | Show RTC time |
| `pci` | Scan PCI devices |
| `uname` | System info (arch, mode) |
| `clear` | Clear screen |
| `reboot` | Halt CPU |
| `help` | Command help |

### Example Session

```
root@myos:/# help
=== MyOS Commands ===
ls      - List files in root directory
cat     - Read file 'motd.txt'
...

root@myos:/# stats
[ INFO ] === Kernel Statistics ===
[ INFO ] Uptime: 1234 ms (1 s)
[ INFO ] Heap: 4096 / 4194304 bytes (0%)
[ INFO ] VFS: 1 / 32 files
[ INFO ] Timer Ticks: 1234
[ INFO ] PCI Devices: 6

root@myos:/# pci
[ INFO ] PCI Device Found – Vendor:0x8086 Device:0x100e
...
```

---

## Architecture

### Directory Structure

```
myos/
├── boot/             # Bootloader (NASM assembly)
├── cpu/              # GDT, IDT, ISR handlers
├── drivers/          # VGA, Keyboard, PCI, ATA, RTC, Timer
├── fs/               # Virtual file system (VFS)
├── kernel/           # Main kernel, memory, stats, logging
├── lib/              # Standard C functions (string, memory)
├── include/          # Headers (vga.h, mem.h, config.h, etc.)
├── Makefile          # Build configuration
├── linker.ld         # Linker script
└── .github/workflows/build.yml  # CI configuration
```

### Boot Flow

1. **boot/boot.s** — Enters 32-bit protected mode, sets up stack, jumps to kernel_main()
2. **kernel/kernel.c** — Initializes GDT, IDT, VFS, timer, and launches shell
3. **Shell Loop** — Waits for keyboard input, parses commands, executes handlers

### Memory Layout

```
0x00000000  ┌─────────────────────┐
            │   Real-mode / BIOS  │ (not used in PM)
0x00007C00  ├─────────────────────┤
            │   Bootloader (512B) │
0x00007E00  ├─────────────────────┤
            │  Kernel + drivers   │ (linked at 0x100000 typically)
            │  Heap (kmalloc)     │ (up to 4 MB)
0x7FFFFFFF  └─────────────────────┘
            │  Video Memory       │ 0xB8000 (VGA text)
            │  I/O Ports          │ 0xC0000-0xFFFFF
0xFFFFFFFF  └─────────────────────┘
```

### Interrupt Handling

- **ISR (Interrupt Service Routines)** — Handles CPU exceptions (division, page fault, etc.)
- **PIC (Programmable Interrupt Controller)** — Routes hardware IRQs (not yet integrated)
- **Handlers dump CPU state** on panic for debugging

---

## Improvements & Fixes

**Version 1.0.1-improved+** includes:

### Critical Fixes (10)
1. Kmalloc heap limit (4 MB) and allocation validation
2. Memory alignment (4-byte, with dynamic kmalloc_a)
3. ISR panic with register dumps
4. ATA drive detection with polling/timeout
5. VGA overflow protection + scrolling
6. Removed undefined sys_reboot() calls
7. String/memory library (memset, memcpy, strcat)
8. VFS validation and file deletion
9. Enhanced shell commands (help, uname, date, stats)
10. Keyboard special keys (ESC, Tab, Enter, Backspace)

### New Features (6)
- Error code enumeration (error.h)
- Runtime assertions and warnings (assert.h)
- Kernel configuration header (config.h)
- Statistics tracking (stats.h, stats.c)
- Verbose command help
- Heap usage reporting

See [IMPROVEMENTS.md](IMPROVEMENTS.md) for full changelog.

---

## Building from Source

### Requirements

- Linux/macOS/WSL with: gcc-multilib, nasm, binutils, xorriso, grub-pc-bin, mtools

### Steps

```bash
# Clone and build
git clone https://github.com/YOUR_USERNAME/myos.git
cd myos
make clean
make

# Run with QEMU
qemu-system-i386 -m 512 -kernel myos.bin

# Or boot ISO
qemu-system-i386 -m 512 -cdrom myos.iso
```

### Makefile Targets

- `make` or `make all` — Build myos.bin and myos.iso
- `make clean` — Remove build artifacts
- `make help` — Show available targets

---

## Future Enhancements

1. **Paging & Virtual Memory** — Implement page tables and memory protection
2. **Interrupts & IRQs** — Register hardware interrupt handlers
3. **Processes & Scheduling** — Task switching with priority levels
4. **Disk I/O** — Real ATA/SATA driver integration
5. **Network Stack** — Basic NIC driver and TCP/IP
6. **User Mode** — Separate kernel/user privilege levels
7. **System Calls** — Standard syscall interface
8. **Module Loading** — Loadable kernel modules

---

## Debugging

### Kernel Panic
When a panic occurs, you'll see:
```
[ KERNEL PANIC ]
Exception: Page Fault
EAX:0x00000000 EBX:0x00000000 ...
EIP:0x00100xxx CS:0x00000008 EFLAGS:0x00000000
```

Use the register dump to trace back to problematic code.

### Building with Debug Symbols
```bash
# Modify CFLAGS in Makefile to include -g, recompile
gcc -m32 -g -ffreestanding ... # then use gdb with qemu
```

### Serial Console
Modify kernel/printk.c to send logs to COM1 (port 0x3F8) for QEMU `-serial stdio`.

---

## References

- **OSDev.org** — Comprehensive OS development wiki
- **Bare Metal x86** — x86 instruction set and registers
- **GNU Binutils** — Linker and assembler docs
- **Intelx86 ISA** — CPU manual (instruction reference)

---

## Contributing

Improvements welcome! Areas to focus on:
- Paging and virtual memory
- Interrupt handlers (keyboard, timer)
- Disk driver (ATA)
- Error handling robustness

---

## License

Public Domain / Educational Use

**Built with 💙 by hobbyist OS developers**

---

**Status**: ✅ Boots, shell functional, PCI scanning works, ~500 LOC optimized kernel  
**Last Updated**: 2026-02-04  
**Build**: Automated CI on GitHub Actions
