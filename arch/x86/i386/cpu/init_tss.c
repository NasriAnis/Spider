#include <mem.h>
#include <gdt.h>
#include <tss.h>

void init_tss(struct GlobalDescriptorTableEntry *gdt_entry){
	uint32_t tss_base = (uint32_t)&tss_entry;
	uint32_t tss_limit = sizeof(tss_entry)-1;

	gdt_entry->SegmentLimit = tss_limit;
	gdt_entry->SegmentBase = tss_base;
	gdt_entry->AccessByte = 0x89;
	gdt_entry->Flags = 0x00;

	memset((uint32_t *)&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0  = KERNEL_DATA_SEGMENT;
	tss_entry.esp0 = KERNEL_CODE_SEGMENT;
}
