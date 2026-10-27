The physical memory manager in the Spider Operating system uses a bitmap
to tell which memory location is free or in use.

First of all the arch specific bootloader calls a functions
`available_mm_register(uintptr_t addr, uint64_t len)` that register an address and length
into a structure `mm_available_regions` that keeps track of how much regions the bootloader
gave with its start address and length of it, these informations get store at the structure
with the highest address specified and an array of address and length.

After finishing registering these regions the kernel call a `pmm_init_regions()` function
that takes the `mm_available_regions` structure parse it and creates a bitmap array that is
located inside the `pmm_bitmap` structure as well as `max_rows` this bitmap has calculate
from the highest address, PAGE_SIZE and numbers of bit per bitmap row "all specified
at `pmm.h`".

To fill this bitmap from the `mm_available_regions` the initialize function takes the highest address
calculate the `max_rows` for a 8 bit column for each row using the `PAGE_SIZE`
then zero out from the `data` pointer address to the `max_rows`.

The kernel address start and length are calculated then a relative row, col to end row, end col
are calculated then these `data` bitmap bits get changed from 0's to 1's marked as used.
