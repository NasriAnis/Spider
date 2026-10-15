.intel_syntax noprefix
.extern cpu_interrupts_handler

.text

isr_common_handler_stub:
	pop eax
	pusha
	push eax
	call cpu_interrupts_handler
	pop eax
	mov al, 0x20          # (EOI)
	out 0x20, al
	popa
	iret

.macro ISR_NOERRCODE num
.global isr\num
isr\num:
	cli
	push 0                # GNU as handles push constants directly
	push \num
	jmp isr_common_handler_stub
.endm

.macro ISR_ERRCODE num
.global isr\num
isr\num:
	cli
	push \num
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