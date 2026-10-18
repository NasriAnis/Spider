C_SOURCES = $(wildcard ../../../kernel/*.c ../../../kernel/shell/*.c ../../../drivers/*/*.c ../../../lib/*.c ../*.c ./*.c ./cpu/*.c ../../../font/*.c)
S_SOURCES = $(wildcard ./*.s ./cpu/*.s ./cpu/interrupts/*.s)

HEADERS   = $(wildcard ../../../kernel/*.h ../../../kernel/shell/*.h ../../../drivers/*/*.h ../../../lib/*.h ../../init.h ../../../font/*.h ../*.h ./*/*.h)

OBJS      = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)

FONT_OBJECT = ../../../font/font_object.o

TARGET    = ../../../build/Spider-IA-32
ISO    = ../../../build/spider.iso
ISODIR = ../../../build/isodir

CC        = gcc
CFLAGS    = -m32 -ffreestanding -Wall -Wextra -g \
			-fno-pie -fno-stack-protector
AS        = as -g --32

LIMINE = /usr/share/limine

all: $(TARGET)

run: iso
	qemu-system-i386 -no-reboot \
		-d int,cpu_reset \
		-cpu pentium3 \
		-m 512M \
		-cdrom $(ISO)

debug: $(TARGET)
	qemu-system-i386 -s -S -no-reboot -cdrom $(ISO) &
	gdb -ex "set disassembly-flavor intel" \
		-ex "target remote localhost:1234" \
		-ex "set architecture i386" \
		-ex "symbol-file $(TARGET)" \
		-ex "break kmain"

iso: $(TARGET)
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
	limine bios-install $(ISO)

$(TARGET): $(OBJS) $(FONT_OBJECT)
	@mkdir -p $(dir $@)
	ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

$(FONT_OBJECT):
	make -C ../../../font i386

clean:
	rm -f boot.o $(OBJS) $(TARGET) $(ISO)
	rm -rf $(ISODIR)
	make -C ../../../font clean
