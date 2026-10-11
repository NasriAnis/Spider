#include "../../../drivers/screen.h"
#include "gdt.h"

static uint8_t GDT[GDT_ENTRIES][GDT_ENTRY_SIZE];
static struct GDTR gdtr;

void encodeGdtEntry(uint8_t *target, struct GDTdata source);
void loadGDT(struct GDTR *gdtr);

void initGDT(void){
	struct GDTdata NullDescriptor = {
        .SegmentBase = 0,
        .SegmentLimit = 0,
        .AccessByte = 0x00,
        .Flags = 0x0
    };
	struct GDTdata KernelSpaceCode = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0x9A,
		.Flags        = 0xC
	};
	struct GDTdata KernelSpaceData = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0x92,
		.Flags        = 0xC
	};
	struct GDTdata UserSpaceCode = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0xFA,
		.Flags        = 0xC
	};
	struct GDTdata UserSpaceData = {
		.SegmentBase  = 0x0,
		.SegmentLimit = 0xFFFFF,
		.AccessByte   = 0xF2,
		.Flags        = 0xC
	};

	encodeGdtEntry(GDT[0], NullDescriptor);
	encodeGdtEntry(GDT[1], KernelSpaceCode);
	encodeGdtEntry(GDT[2], KernelSpaceData);
	encodeGdtEntry(GDT[3], UserSpaceCode);
	encodeGdtEntry(GDT[4], UserSpaceData);

	gdtr.limit = sizeof(GDT) - 1;
	gdtr.base  = (uint32_t)GDT;

	loadGDT(&gdtr);
}

void loadGDT(struct GDTR *gdtr){
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

void encodeGdtEntry(uint8_t *target, struct GDTdata source){
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