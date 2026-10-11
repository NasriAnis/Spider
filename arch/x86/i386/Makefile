C_SOURCES = $(wildcard ../../../kernel/*.c ../../../drivers/*.c ../../../lib/*.c ./*.c)
HEADERS   = $(wildcard ../../../kernel/*.h ../../../drivers/*.h ../../../lib/*.h ../../init.h ./*.h)
OBJS      = $(C_SOURCES:.c=.o)

TARGET    = ../../build/Spider-IA-32

CC        = gcc
CFLAGS    = -m32 -ffreestanding -Wall -Wextra -g \
			-fno-pie
AS        = as -g --32

all: $(TARGET)

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

debug: $(TARGET)
	qemu-system-i386 -s -S -kernel $(TARGET) &
	gdb -ex "set disassembly-flavor intel" \
		-ex "target remote localhost:1234" \
		-ex "set architecture i386" \
		-ex "symbol-file $(TARGET)" \
		-ex "break kmain"

$(TARGET): boot.o $(OBJS)
	@mkdir -p $(dir $@)
	ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -f boot.o $(OBJS) $(TARGET)