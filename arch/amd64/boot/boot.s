.intel_syntax noprefix

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    cli
    cld
    lea rsp, [rip + stack_top]
    xor ebp, ebp
    call kmain
1:  hlt
    jmp 1b

.size _start, . - _start