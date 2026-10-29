.intel_syntax noprefix
.global keyboard_handler_stub
.extern keyboard_handler

keyboard_handler_stub:
	pusha
	call keyboard_handler
	mov al, 0x20 # (EOI)
	out 0x20, al
	popa
	iret