.intel_syntax noprefix
.extern cpu_interrupts_handler
.global default_isr_stub

.text

# Default fallback handler for unmapped interrupt vectors
default_isr_stub:
	cli
	hlt
	jmp default_isr_stub

isr_common_handler_stub:
	# Save all 64-bit general-purpose registers
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	# System V ABI: Pass pointer to saved registers structure as 1st argument (RDI)
	mov rdi, rsp
	call cpu_interrupts_handler

	# Restore general-purpose registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	# Clean up vector number and error code (2 * 8 bytes = 16 bytes)
	add rsp, 16

	# Return from 64-bit interrupt
	iretq

.macro ISR_NOERRCODE num
.global isr\num
isr\num:
	cli
	push 0                 # Dummy error code
	push \num              # Interrupt vector number
	jmp isr_common_handler_stub
.endm

.macro ISR_ERRCODE num
.global isr\num
isr\num:
	cli
	# CPU automatically pushed the error code onto stack
	push \num              # Interrupt vector number
	jmp isr_common_handler_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE   30
ISR_NOERRCODE 31