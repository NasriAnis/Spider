# TODO
- [ ] Implement a Kerror handler function
- [ ] Implement User space
- [ ] Expend to ARM architecture

# In progress
- [ ] Expend to amd64 architecture
- [ ] Setup a filesystem

# On Hold
- [ ] Abstraction from PMM with malloc
- [ ] Processes abstraction
- [ ] Setup Paging

# Done
- [x] Setup linear framebuffer text printing abstraction
- [x] Parse multiboot structures to get FB and PMM addresses
- [x] Setup GDT and TSS tables
- [x] Setup interupts for CPU
- [x] Setup a keyboard driver and interrupt handler
- [x] Setup a display driver abstraction
- [x] PMM Memory allocator

# Note
For now all Done TODOs are for the i386 architeture. Expanding to amd64 and ARM will be considered later.