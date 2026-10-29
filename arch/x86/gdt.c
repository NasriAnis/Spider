#include <gdt.h>
#include <int.h>
#include <io.h>

void load_gdt(struct GDTR *gdtr){
	asm(
		"lgdt (%0)" :: "r" (gdtr)
	);
	asm volatile(
		"mov $0x10, %%ax\n"
		"mov %%ax, %%ds\n"
		"mov %%ax, %%es\n"
		"mov %%ax, %%fs\n"
		"mov %%ax, %%gs\n"
		"mov %%ax, %%ss\n"
		"ljmp $0x08, $1f\n"
		"1:\n"
		: : : "ax", "memory"
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