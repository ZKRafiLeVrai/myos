#include "idt.h" // Le Makefile gère le chemin avec -I./include

idt_entry_t idt[256];
idt_ptr_t idt_reg;

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)((handler) & 0xFFFF);
    idt[n].sel = 0x08;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = (uint16_t)(((handler) >> 16) & 0xFFFF);
}

void idt_install() {
    idt_reg.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_reg.base = (uint32_t)&idt;
    
    // Initialisation par défaut ici (Exemple pour l'horloge ou le clavier plus tard)
    
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_reg));
}