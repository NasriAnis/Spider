.intel_syntax noprefix

// layout for Multiboot2 header
.set MAGIC,    0x1BADB002
.set FLAGS,    (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// setup the stack 16 KiB
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

// entry
.section .text
.global _start
.type _start, @function
_start:
	mov esp, offset stack_top
	call kmain
   	cli
1:	hlt
	jmp 1b

.size _start, . - _start