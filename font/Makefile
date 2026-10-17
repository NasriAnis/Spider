FONT = default8x16.psfu
FONT_OBJECT = font_object.o

i386:
	objcopy -O elf32-i386 -B i386 -I binary $(FONT) $(FONT_OBJECT)

clean:
	rm $(FONT_OBJECT)