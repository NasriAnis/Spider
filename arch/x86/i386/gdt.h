#include "../../../lib/int.h"

struct GDTdata {
	uint32_t SegmentBase;
	uint32_t SegmentLimit; // low 20 bits used
	uint8_t AccessByte;
	uint8_t Flags;         // low 4 bits used
};

#define GDT_ENTRIES 5
#define GDT_ENTRY_SIZE 8

struct __attribute__((packed)) GDTR {
	uint16_t limit;
	uint32_t base;
};

/*
	* Initialize GDT table to memory
	* and load its limit/base to gdtr
	* note that this GDT table specifies
	* a flat model setup.

	* In memory GDT table layout:

	* 00 00 00 00 00 00 00 00    # null
	* ff ff 00 00 00 9a cf 00    # kernel code
	* ff ff 00 00 00 92 cf 00    # kernel data
	* ff ff 00 00 00 fa cf 00    # user code
	* ff ff 00 00 00 f2 cf 00    # user data
*/
void initGDT(void);