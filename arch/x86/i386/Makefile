ROOT := ../../..
SRC_DIRS := $(ROOT)/kernel $(ROOT)/drivers $(ROOT)/lib $(ROOT)/font $(ROOT)/shell

C_SOURCES := $(sort \
    $(shell find $(SRC_DIRS) -name '*.c') \
    $(wildcard ../*.c) \
    $(shell find . -name '*.c'))
S_SOURCES := $(sort $(shell find . -name '*.s'))

OBJS      = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)
DEPS = $(OBJS:.o=.d)

FONT_OBJECT = ../../../font/font_object.o

TARGET    = ../../../build/Spider-IA-32
ISO    = ../../../build/spider.iso
ISODIR = ../../../build/isodir

CC        = gcc
CFLAGS    = -m32 -ffreestanding -Wall -Wextra -g \
			-fno-pie -fno-stack-protector \
			-I$(ROOT) -I$(ROOT)/lib -I$(ROOT)/drivers -I$(ROOT)/arch/include -I$(ROOT)/arch/x86/include -I$(ROOT)/shell \
			-MMD -MP -mno-sse -mno-sse2 -mno-mmx -mno-3dnow
AS        = as -g --32

LIMINE = $(ROOT)/deps/limine
LIMINE_BIN = $(LIMINE)/limine

-include $(DEPS)

all: $(TARGET)

run: iso
	qemu-system-i386 -no-reboot -no-shutdown \
		-d int,cpu_reset \
		-cpu pentium3 \
		-m 512M \
		-cdrom $(ISO)

debug: iso
	qemu-system-i386 -s -S -no-reboot -cdrom $(ISO) -d int,cpu_reset -no-shutdown -D qemu.log &
	gdb -ex "set disassembly-flavor intel" \
		-ex "target remote localhost:1234" \
		-ex "set architecture i386" \
		-ex "symbol-file $(TARGET)" \
		-ex "break kmain"

iso: $(TARGET) $(LIMINE_BIN)
	rm -rf $(ISODIR)
	mkdir -p $(ISODIR)/boot/limine $(ISODIR)/EFI/BOOT
	cp $(TARGET) $(ISODIR)/boot/spider.elf
	cp limine.conf $(ISODIR)/boot/limine/
	cp $(LIMINE)/limine-bios.sys $(LIMINE)/limine-bios-cd.bin $(LIMINE)/limine-uefi-cd.bin $(ISODIR)/boot/limine/
	cp $(LIMINE)/BOOTIA32.EFI $(LIMINE)/BOOTX64.EFI $(ISODIR)/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISODIR) -o $(ISO)
	$(LIMINE_BIN) bios-install $(ISO)

$(TARGET): $(OBJS) $(FONT_OBJECT)
	@mkdir -p $(dir $@)
	ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

$(FONT_OBJECT):
	make -C ../../../font i386

clean:
	rm -f boot.o $(OBJS) $(TARGET) $(ISO) $(DEPS)
	rm -rf $(ISODIR)
	make -C ../../../font clean
