ROOT := ../..
SRC_DIRS := $(ROOT)/kernel $(ROOT)/drivers $(ROOT)/lib $(ROOT)/font $(ROOT)/shell

C_SOURCES := $(sort \
    $(shell find $(SRC_DIRS) -name '*.c') \
    $(shell find . -name '*.c'))
S_SOURCES := $(sort \
    $(shell find $(SRC_DIRS) -name '*.s') \
    $(shell find . -name '*.s'))

OBJS      = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)
DEPS = $(OBJS:.o=.d)

FONT_OBJECT = $(ROOT)/font/font_object.o

TARGET    = $(ROOT)/build/Spider-x86_64
ISO    = $(ROOT)/build/spider.iso
ISODIR = $(ROOT)/build/isodir

CC        = gcc
CFLAGS    = -m64 -march=x86-64 -ffreestanding -Wall -Wextra -g \
			-fno-pie -fno-pic -fno-stack-protector \
			-I$(ROOT) -I$(ROOT)/lib -I$(ROOT)/drivers -I$(ROOT)/arch/include \
		 	-I$(ROOT)/arch/amd64/include -I$(ROOT)/shell -I$(ROOT)/deps \
			-MMD -MP -mcmodel=kernel -mno-red-zone
AS        = as -g --64

LIMINE = $(ROOT)/deps/limine
LIMINE_BIN = $(LIMINE)/limine

-include $(DEPS)

all: $(TARGET)

run: iso
	qemu-system-x86_64 -no-reboot -no-shutdown -D qemu.log \
		-d int,cpu_reset \
		-cpu qemu64 \
		-m 16G \
		-cdrom $(ISO)
	$(MAKE) clean

debug: iso
	qemu-system-x86_64 -s -S -no-reboot -cdrom $(ISO) -d int,cpu_reset -no-shutdown -D qemu.log &
	gdb -ex "set disassembly-flavor intel" \
		-ex "target remote localhost:1234" \
		-ex "set architecture i386:x86-64" \
		-ex "symbol-file $(TARGET)" \
		-ex "break kmain"

iso: $(TARGET) $(LIMINE_BIN)
	rm -rf $(ISODIR)
	mkdir -p $(ISODIR)/boot/limine $(ISODIR)/EFI/BOOT
	cp $(TARGET) $(ISODIR)/boot/spider.elf
	cp limine.conf $(ISODIR)/boot/limine/
	cp $(LIMINE)/limine-bios.sys $(LIMINE)/limine-bios-cd.bin $(LIMINE)/limine-uefi-cd.bin $(ISODIR)/boot/limine/
	cp $(LIMINE)/BOOTX64.EFI $(ISODIR)/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISODIR) -o $(ISO)
	$(LIMINE_BIN) bios-install $(ISO)

$(TARGET): $(OBJS) $(FONT_OBJECT)
	@mkdir -p $(dir $@)
	ld -m elf_x86_64 -T linker.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

$(FONT_OBJECT):
	make -C $(ROOT)/font x86_64

clean:
	rm -f boot.o $(OBJS) $(TARGET) $(ISO) $(DEPS)
	rm -rf $(ISODIR)
	make -C $(ROOT)/font clean