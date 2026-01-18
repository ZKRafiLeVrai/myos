ENTRY(_start)

SECTIONS
{
    /* On commence à 1Mo, standard pour le multiboot */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot) /* FORCE LE HEADER AU DÉBUT */
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
}