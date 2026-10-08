C_SOURCES = $(wildcard ../../kernel/*.c ../../drivers/*.c ../../lib/*.c)
HEADERS   = $(wildcard ../../kernel/*.h ../../drivers/*.h ../../lib/*.h)
OBJS      = $(C_SOURCES:.c=.o)

TARGET    = ../../build/Spider-IA-32

CC        = gcc
CFLAGS    = -m32 -ffreestanding -Wall -Wextra -g \
			-fno-pie
AS        = as --32

all: $(TARGET)

run: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

$(TARGET): boot.o $(OBJS)
	@mkdir -p $(dir $@)
	ld -m elf_i386 -T linker.ld -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -f boot.o $(OBJS) $(TARGET)