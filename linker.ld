ENTRY(_start)

SECTIONS
{
    /* On commence à 1Mo */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        /* Le header Multiboot DOIT être dans les 8 premiers Ko du fichier */
        KEEP(*(.multiboot))
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
end = .;  /* AJOUTE CETTE LIGNE JUSTE AVANT LA DERNIÈRE ACCOLADE */
}