#include <arch.h>
#include <io.h>
#include "boot/limine.h"
#include <drivers/io/keyboard.h>
#include <gdt.h>
#include <tss.h>
#include <idt.h>
#include <pic.h>
#include <isr.h>
#include <pio.h>
#include <msr.h>

#define IA32_APIC_BASE_MSR 0x1B
void disable_lapic_and_enable_pic(void) {
    /* 1. Clear Bit 11 of IA32_APIC_BASE MSR to disable Local APIC hardware */
    uint64_t apic_base = rdmsr(IA32_APIC_BASE_MSR);
    apic_base &= ~(1ULL << 11);
    wrmsr(IA32_APIC_BASE_MSR, apic_base);

    /* 2. Force IMCR to legacy 8259 PIC mode via ports 0x22 / 0x23 */
    port_byte_out(0x22, 0x70);
    port_byte_out(0x23, 0x00);
}

void init_arch(void) {
	limine_parse_responses();
	putsnl("[ok] Parse Limine tables successfully");

	/* Make sure framebuffer address, width, height, and pitch are set */
	// init_fb_driver(fb_response->address, fb_response->width, fb_response->height, fb_response->pitch);

	init_gdt();
	init_tss();
	putsnl("[ok] Init GDT and TSS successfully");

	/* 
		* TODO_LAPIC: [LAPIC_DISABLE_HACK]
		* Limine boots with the Local APIC globally enabled by default. This routes legacy 
		* 8259 PIC signals to LAPIC LINT0 (which is masked), preventing IRQs like IRQ1 
		* (Keyboard) from reaching the CPU core.
		* 
		* We temporarily power down the LAPIC and route signals to legacy PIC mode here.
		* REMOVE / REVISIT THIS FUNCTION when implementing modern APIC / IOAPIC support!
		*/
	disable_lapic_and_enable_pic();

	init_idt();
	cpu_interrupts_setup();

	ps2_keyboard_init();

	/* Drain any stale PS/2 bytes before enabling interrupts */
	while (port_byte_in(0x64) & 1) {
		port_byte_in(0x60);
	}


	__asm__ volatile ("sti");
	putsnl("[ok] Init IDT tables successfully");
}