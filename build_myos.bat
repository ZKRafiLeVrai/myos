@echo off
setlocal enabledelayedexpansion

REM Requirements: nasm, gcc (with -m32 support), ld (binutils), grub-mkrescue (optional)
REM Adjust paths to tools if needed, e.g. set NASM=C:\path\to\nasm.exe

set CC=gcc
set AS=nasm
set LD=ld
set CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -fno-stack-protector -I./include
set LDFLAGS=-m elf_i386 -T linker.ld -nostdlib

if not exist boot\boot.s (
  echo boot/boot.s missing
  exit /b 1
)

REM object list
set OBJS=boot\boot.o kernel\kernel.o kernel\printk.o kernel\mem.o cpu\gdt.o cpu\idt.o cpu\isr.o drivers\vga.o drivers\kbd.o drivers\pci.o drivers\ata.o drivers\rtc.o drivers\timer.o fs\vfs.o lib\string.o io.o

REM compile .c files
for %%f in (kernel\kernel.c kernel\printk.c kernel\mem.c cpu\gdt.c cpu\idt.c cpu\isr.c drivers\vga.c drivers\kbd.c drivers\pci.c drivers\ata.c drivers\rtc.c drivers\timer.c fs\vfs.c lib\string.c io.c) do (
  echo Compiling %%f
  %CC% %CFLAGS% -c %%f -o %%~dpn.o
  if errorlevel 1 (
    echo Failed to compile %%f
    exit /b 1
  )
)

REM assemble boot
echo Assembling boot\boot.s
%AS% -f elf32 boot\boot.s -o boot\boot.o
if errorlevel 1 (
  echo Failed to assemble boot\boot.s
  exit /b 1
)

REM link
echo Linking myos.bin
%LD% %LDFLAGS% -o myos.bin %OBJS%
if errorlevel 1 (
  echo Link failed
  exit /b 1
)

REM create isodir and grub config (requires mkdir -p equivalent)
if exist isodir rd /s /q isodir
mkdir isodir
mkdir isodir\boot
mkdir isodir\boot\grub
copy /y myos.bin isodir\boot\myos.bin >nul

echo set timeout=0> isodir\boot\grub\grub.cfg
>> isodir\boot\grub\grub.cfg echo menuentry "MyOS Pro HD (Protected Mode)" {
>> isodir\boot\grub\grub.cfg echo   multiboot /boot/myos.bin
>> isodir\boot\grub\grub.cfg echo   boot
>> isodir\boot\grub\grub.cfg echo }

REM optionally create ISO if grub-mkrescue available
where grub-mkrescue >nul 2>&1
if %errorlevel%==0 (
  echo Creating myos.iso with grub-mkrescue
  grub-mkrescue -o myos.iso isodir
) else (
  echo grub-mkrescue not found; myos.bin created and isodir prepared.
)

echo Build finished.
endlocal