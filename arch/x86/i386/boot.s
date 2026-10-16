.intel_syntax noprefix

// layout for Multiboot2 header
.set MB2_MAGIC,    0xE85250D6
.set MB2_ARCH,     0                 # 0 = i386 protected mode
.set MB2_LENGTH,   (header_end - header_start)
.set MB2_CHECKSUM, -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

.section .multiboot
.align 8
header_start:
	.long MB2_MAGIC
	.long MB2_ARCH
	.long MB2_LENGTH
	.long MB2_CHECKSUM

	# end tag
	.align 8
	.short 0        # type
	.short 0        # flags
	.long 8         # size
header_end:

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
	cli
	cld
	mov esp, offset stack_top
	push ebx            # info pointer
	push eax            # magic
	call boot_check
	1:  hlt
	jmp 1b

.size _start, . - _start