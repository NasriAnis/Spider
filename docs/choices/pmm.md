The physical memory manager in the Spider Operating system uses a bitmap
to tell which memory location is free or in use.

First of all the arch specific bootloader calls a functions
`available_mm_register(entry->addr, entry->len);` that register an address and length
into a structure called `mm_available_regions` that keeps track of how much regions
the bootloader gave as well as the start address and length of each region stored into
an array inside the structure, the highest region is kept stored in that same structure.

After finishing registering these regions the kernel call a `pmm_init_regions()` function
that takes the `mm_available_regions` structure parse it and creates a bitmap array that is
located inside the `pmm_bitmap` structure as well as `max_rows` this bitmap has.

To fill this bitmap from the `mm_available_regions` the initialize function takes the highest address
calculate the `max_rows` (padding doest count) for a 8 bit column for each row using the `PAGE_SIZE`
then zero out from the `data` pointer address to the `max_rows`.

The kernel address start and length are calculated then a relative row to row and col to col
`data` bitmap bits get changed with 1 marked as used.
