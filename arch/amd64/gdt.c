#include <gdt.h>
#include <int.h>
#include <io.h>

void load_gdt(struct GDTR *gdtr) {
	asm volatile(
		"lgdt (%0)\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"
		"mov %%ax, %%ss\n\t"
		"pushq $0x08\n\t"
		"leaq 1f(%%rip), %%rax\n\t"
		"pushq %%rax\n\t"
		"lretq\n\t"
		"1:\n\t"
		:
		: "r" (gdtr)
		: "rax", "memory"
		);
}

void encode_gdt_entry(uint8_t *target, struct GlobalDescriptorTableEntry source){
	if (source.SegmentLimit > 0xFFFFF) {
		// [FIX] make a kernel error handler
		eputsnl("GDT cannot encode limits larger than 0xFFFFF");
		asm( "hlt" );
	};

	// Encode the limit
	target[0] = source.SegmentLimit & 0xFF;
	target[1] = (source.SegmentLimit >> 8) & 0xFF;
	target[6] = ((source.SegmentLimit >> 16) & 0x0F) | (source.Flags << 4);

	// Encode the base
	target[2] = source.SegmentBase & 0xFF;
	target[3] = (source.SegmentBase >> 8) & 0xFF;
	target[4] = (source.SegmentBase >> 16) & 0xFF;
	target[7] = (source.SegmentBase >> 24) & 0xFF;

	// Encode Access Bytes
	target[5] = source.AccessByte;
}