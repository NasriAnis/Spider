ENTRY(_start)
SECTIONS
{
    . = 2M;
    .text      : { *(.multiboot) *(.text) }
    .rodata    : { *(.rodata) }
    .data      : { *(.data) }
    .bss       : { *(.bss) *(COMMON) }
}