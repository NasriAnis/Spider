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

	// INFORMATION REQUEST TAG
	.align 8
	.short 1        # type = 1 (Information Request)
	.short 0        # flags (0 = mandatory, or 1 = optional)
	.long 16        # size of this tag (8 bytes header + 2 requests * 4 bytes)
	.long 6         # Request type 6: Memory Map (mmap)
	.long 8         # Request type 8: Framebuffer

	// FRAMEBUFFER TAG
	.align 8
	.short 5        # type = 5 (Framebuffer tag)
	.short 0        # flags (0 = mandatory, 1 = optional)
	.long 20        # size of this tag (20 bytes total)
	.long 1024      # preferred width
	.long 768       # preferred height
	.long 32        # preferred bits per pixel (depth)

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
	call multiboot2_check
	call kmain
	1:  hlt
	jmp 1b

.size _start, . - _start