# Chemins des outils
CC = gcc
AS = nasm
LD = ld

# CFLAGS : -m32 pour le mode protégé, -fno-stack-protector pour éviter les crashs sur vrai PC
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector -I./include
# LDFLAGS : -m elf_i386 est crucial pour le linker
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Liste complète des objets (Incluant le nouveau gdt_flush.o)
OBJ = boot/boot.o \
      cpu/gdt_flush.o \
      cpu/gdt.o \
      cpu/idt.o \
      cpu/isr.o \
      kernel/kernel.o \
      kernel/printk.o \
      kernel/mem.o \
      drivers/vga.o \
      drivers/kbd.o \
      drivers/pci.o \
      drivers/ata.o \
      drivers/rtc.o \
      drivers/timer.o \
      fs/vfs.o \
      fs/editor.o \
      lib/string.o \
      io.o
# Cible principale
all: myos.iso

# Compilation des fichiers C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation du bootloader (Assembleur)
boot/boot.o: boot/boot.s
	$(AS) -felf32 boot/boot.s -o boot/boot.o

# Compilation du GDT Flush (Indispensable pour vrai PC)
cpu/gdt_flush.o: cpu/gdt_flush.s
	$(AS) -felf32 cpu/gdt_flush.s -o cpu/gdt_flush.o

# Liaison du noyau
myos.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o myos.bin $(OBJ)

# Création de l'ISO bootable
myos.iso: myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	@echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	@echo 'menuentry "MyOS Pro HD (Protected Mode)" {' >> isodir/boot/grub/grub.cfg
	@echo '  multiboot /boot/myos.bin' >> isodir/boot/grub/grub.cfg
	@echo '  boot' >> isodir/boot/grub/grub.cfg
	@echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

# Nettoyage
clean:
	rm -rf $(OBJ) myos.bin myos.iso isodir