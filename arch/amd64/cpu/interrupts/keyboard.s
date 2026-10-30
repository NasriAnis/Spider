.intel_syntax noprefix
.global keyboard_handler_stub
.extern keyboard_handler
.extern pic_send_eoi

.text

keyboard_handler_stub:
	# Save all 64-bit registers
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

	# Clear Direction Flag for System V AMD64 ABI
	cld

	# Call C Keyboard Handler
	call keyboard_handler

	# Send EOI for IRQ1 (1st argument in RDI = 1)
	mov rdi, 1
	call pic_send_eoi

	# Restore registers
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

	# Return from 64-bit interrupt
	iretq