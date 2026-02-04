CC = gcc
AS = nasm
LD = ld

# CFLAGS pour Mode Protégé Qualité HD
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector -I./include
# LDFLAGS corrigé pour ld (format i386)
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

OBJ = boot/boot.o \
      kernel/kernel.o \
      kernel/printk.o \
      kernel/mem.o \
      kernel/stats.o \
      kernel/shell.o \
      kernel/shell_input.o \
      cpu/gdt.o \
      cpu/idt.o \
      cpu/isr.o \
      drivers/vga.o \
      drivers/kbd.o \
      drivers/pci.o \
      drivers/ata.o \
      drivers/rtc.o \
      drivers/timer.o \
      fs/vfs.o \
      lib/string.o \
      io.o

all: myos.iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.s
	$(AS) -felf32 boot/boot.s -o boot/boot.o

myos.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o myos.bin $(OBJ)

myos.iso: myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	echo 'menuentry "MyOS Pro HD (Protected Mode)" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/myos.bin' >> isodir/boot/grub/grub.cfg
	echo '  boot' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

clean:
	rm -rf $(OBJ) myos.bin myos.iso isodir