; Configuration Multiboot
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .multiboot
align 4
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main
extern gdt_install

_start:
    ; 1. Désactiver les interruptions immédiatement (Crucial sur vrai PC)
    cli
    
    ; 2. Initialiser la pile
    mov esp, stack_top
    mov ebp, esp

    ; 3. Remettre les flags à zéro et Direction Flag à 0 (cld)
    ; Sans cld, strcmp/strcpy peuvent faire n'importe quoi en C
    cld
    push 0
    popf

    ; 4. Installer la GDT
    call gdt_install

    ; 5. Saut vers le noyau C
    push eax ; Pousse le magic multiboot (optionnel mais propre)
    push ebx ; Pousse l'adresse de la structure info multiboot
    call kernel_main

    ; 6. Sécurité
.hang:
    hlt
    jmp .hang