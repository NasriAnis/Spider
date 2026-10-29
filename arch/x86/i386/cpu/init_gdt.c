#include <io.h>
#include <gdt.h>
#include "tss.h"

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

	struct GlobalDescriptorTableEntry TssDescriptor;
	init_tss(&TssDescriptor);
	encode_gdt_entry(GDT[5], TssDescriptor);
	load_task_register();
}
