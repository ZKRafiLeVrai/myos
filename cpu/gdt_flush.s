[BITS 32]
global gdt_flush
extern gp

gdt_flush:
    mov eax, [esp+4]  ; Récupère le pointeur vers la structure gp passé en argument
    lgdt [eax]        ; Charge la GDT dans le processeur

    mov ax, 0x10      ; 0x10 est l'offset du segment de données (2ème entrée de la GDT * 8)
    mov ds, ax        ; On recharge tous les registres de données
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Le registre CS (Code Segment) ne peut pas être modifié avec un MOV.
    ; On doit faire un "Far Jump" (Saut lointain) pour le forcer à 0x08.
    jmp 0x08:.flush

.flush:
    ret