MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_MAGIC         equ 0x1BADB002
MBOOT_FLAGS         equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_MAGIC + MBOOT_FLAGS)

[BITS 32]
section .multiboot
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push eax            ; Magic number multiboot
    push ebx            ; Adresse de la structure d'info multiboot
    call kernel_main
    cli
.hang: hlt
    jmp .hang

section .bss
align 16
stack_bottom: resb 16384 ; 16 KB pile kernel
stack_top: