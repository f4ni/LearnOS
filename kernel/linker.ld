
ENTRY(kernel_entry)

SECTIONS
{
    /* Kernel will be loaded at physical address 0x1000 */
    . = 0x1000;

    .text :         /* Code section */
    {
        *(.text*)
    }

    .rodata :       /* Read-only data */
    {
        *(.rodata*)
    }

    .data :         /* Initialized data */
    {
        *(.data*)
    }

    .bss :         /* Uninitialized data (BSS) */
    {
        *(COMMON)
        *(.bss*)
    }
}
