# --- Configuration du Compilateur ---
CC = gcc
AS = nasm
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector -I./include
LDFLAGS = -m32 -T linker.ld -nostdlib -lgcc

# --- Liste des Objets ---
# On liste les dossiers pour être sûr de ne rien oublier
OBJ = boot/boot.o \
      kernel/kernel.o \
      kernel/mem.o \
      cpu/gdt.o \
      cpu/idt.o \
      cpu/isr.o \
      drivers/vga.o \
      drivers/kbd.o \
      drivers/timer.o \
      fs/vfs.o \
      lib/string.o \
      lib/stdio.o \
      io.o

# --- Cibles Principales ---

all: myos.iso

# Compilation des fichiers C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation du fichier de Boot (Assembleur)
boot/boot.o: boot/boot.s
	$(AS) -felf32 boot/boot.s -o boot/boot.o

# Liaison du Kernel (Linking)
myos.bin: $(OBJ)
	$(CC) $(LDFLAGS) -o myos.bin $(OBJ)

# Création de l'image ISO
myos.iso: myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	@echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	@echo 'set default=0' >> isodir/boot/grub/grub.cfg
	@echo 'menuentry "MyOS Linux" {' >> isodir/boot/grub/grub.cfg
	@echo '  multiboot /boot/myos.bin' >> isodir/boot/grub/grub.cfg
	@echo '  boot' >> isodir/boot/grub/grub.cfg
	@echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

# Nettoyage des fichiers temporaires
clean:
	rm -rf $(OBJ) myos.bin myos.iso isodir