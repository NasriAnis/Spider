#include <isr.h>
#include <idt.h>
#include <int.h>
#include <io.h>

/* Saved register frame pushed by assembly stub (isr_common_handler_stub) */
struct registers {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
	uint64_t int_no, err_code;
	uint64_t rip, cs, rflags, rsp, ss;
};

typedef void (*isr_stub_t)(void);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

static const isr_stub_t isr_stub_table[32] = {
	isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,
	isr8,  isr9,  isr10, isr11, isr12, isr13, isr14, isr15,
	isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
	isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
};

void cpu_interrupts_setup(void) {
	for (int i = 0; i < 32; i++) {
		set_idt_gate(i, (uintptr_t)isr_stub_table[i], 0, 0x8E);
	}
}

/* Exception messages indexed by interrupt vector number */
const char *exception_messages[32] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void cpu_interrupts_handler(struct registers *regs) {
	if (regs->int_no < 32) {
		eputs("[x] Interrupt occurred: ");
		eputsnl(exception_messages[regs->int_no]);
	} else {
		eputsnl("[x] Unknown Interrupt");
	}

	__asm__ volatile ("cli; hlt");
}