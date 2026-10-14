#include "../../../kernel/console.h"
#include "gdt.h"

static uint8_t GDT[GDT_ENTRIES][GDT_ENTRY_SIZE];
static struct GDTR gdtr;

void encode_gdt_entry(uint8_t *target, struct GlobalDescriptorTableEntry source);
void load_gdt(struct GDTR *gdtr);

void init_gdt(void){
	struct GlobalDescriptorTableEntry NullDescriptor = {
		.SegmentBase = 0,
		.SegmentLimit = 0,
		.AccessByte = 0x00,
		.Flags = 0x0
	};
	struct GlobalDescriptorTableEntry KernelSpaceCode = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0x9A,
		.Flags        = 0xC
	};
	struct GlobalDescriptorTableEntry KernelSpaceData = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0x92,
		.Flags        = 0xC
	};
	struct GlobalDescriptorTableEntry UserSpaceCode = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0xFA,
		.Flags        = 0xC
	};
	struct GlobalDescriptorTableEntry UserSpaceData = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0xF2,
		.Flags        = 0xC
	};

	encode_gdt_entry(GDT[0], NullDescriptor);
	encode_gdt_entry(GDT[1], KernelSpaceCode);
	encode_gdt_entry(GDT[2], KernelSpaceData);
	encode_gdt_entry(GDT[3], UserSpaceCode);
	encode_gdt_entry(GDT[4], UserSpaceData);

	gdtr.limit = sizeof(GDT) - 1;
	gdtr.base  = (uint32_t)GDT;

	load_gdt(&gdtr);
}

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
		eprintnl("GDT cannot encode limits larger than 0xFFFFF");
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