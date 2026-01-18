#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Structure d'une entrée dans la table (Gate)
struct idt_entry_struct {
    uint16_t low_offset; // Bas 16 bits de l'adresse de l'ISR
    uint16_t sel;        // Sélecteur de segment de code
    uint8_t  always0;    // Doit être 0
    uint8_t  flags;      // Drapeaux (Présence, Privilège, Type)
    uint16_t high_offset;// Haut 16 bits de l'adresse de l'ISR
} __attribute__((packed));

// Structure pour le registre IDTR (lidt)
struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// On définit les types pour que idt.c les reconnaisse
typedef struct idt_entry_struct idt_entry_t;
typedef struct idt_ptr_struct idt_ptr_t;

// Définition de la structure des registres pour l'ISR
struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void idt_install();
void set_idt_gate(int n, uint32_t handler);

#endif