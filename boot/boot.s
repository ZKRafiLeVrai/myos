; --- Configuration Multiboot ---
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_MAGIC         equ 0x1BADB002
MBOOT_FLAGS         equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_MAGIC + MBOOT_FLAGS)

[BITS 32]

; Section Multiboot : doit être au tout début du binaire via le linker
section .multiboot
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

section .text
global _start
extern kernel_main  ; Déclare la fonction C définie dans kernel.c

_start:
    ; Initialisation de la pile (stack)
    mov esp, stack_top

    ; Appel du noyau C
    call kernel_main

    ; Boucle de sécurité si kernel_main retourne
    cli
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB de pile
stack_top: