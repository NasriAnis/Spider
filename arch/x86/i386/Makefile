C_SOURCES = $(wildcard ../../../kernel/*.c ../../../kernel/shell/*.c ../../../drivers/*/*.c ../../../lib/*.c ../*.c ./*.c ./cpu/*c)
S_SOURCES = $(wildcard ./*.s ./cpu/*.s ./cpu/interrupts/*.s)
HEADERS   = $(wildcard ../../../kernel/*.h ../../../kernel/shell/*.h ../../../drivers/*/*.h ../../../lib/*.h ../../init.h ../*.h ./*/*.h)
OBJS      = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)

TARGET    = ../../../build/Spider-IA-32
ISO    = ../../../build/spider.iso
ISODIR = ../../../build/isodir

CC        = gcc
CFLAGS    = -m32 -ffreestanding -Wall -Wextra -g \
			-fno-pie -fno-stack-protector
AS        = as -g --32

all: $(TARGET)

run: iso
	qemu-system-i386 -no-reboot -d int,cpu_reset -cdrom $(ISO)

debug: $(TARGET)
	qemu-system-i386 -s -S -no-reboot -cdrom $(ISO) &
	gdb -ex "set disassembly-flavor intel" \
		-ex "target remote localhost:1234" \
		-ex "set architecture i386" \
		-ex "symbol-file $(TARGET)" \
		-ex "break kmain"

iso: $(TARGET)
	mkdir -p $(ISODIR)/boot/grub
	cp $(TARGET) $(ISODIR)/boot/spider.elf
	printf 'set timeout=0\nset default=0\nmenuentry \
		"Spider" {\n\tmultiboot2 /boot/spider.elf\n\tboot\n}\n' \
	 	> $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISODIR)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -f boot.o $(OBJS) $(TARGET) $(ISO)
	rm -rf $(ISODIR)