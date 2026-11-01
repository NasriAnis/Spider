#include <io.h>
#include <int.h>
#include <stdarg.h>
#include <kernel/console.h>

char *convert(unsigned int num, int base) {
	static char Representation[]= "0123456789ABCDEF";
	static char buffer[50];
	char *ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = Representation[num%base];
		num /= base;
	} while(num != 0);

	return(ptr);
}

void printf(char *format, ...){
	va_list arg;
	va_start(arg, format);

	unsigned int i;
	char *s;

	char *traverse;

	for(traverse = format; *traverse != '\0'; traverse++) {
		while (*traverse != '%') {
			if (*traverse=='\n'){
				putc('\n');
			} else {
				putc(*traverse);
			}
			traverse++;
		}

		traverse++;

		switch (*traverse) {
			case 's':
				s = va_arg(arg,char *);
				puts(s);
				break;
			case 'c':
				i = va_arg(arg,int);
				putc(i);
				break;
			case 'x':
				i = va_arg(arg,unsigned int);
				puts("0x");
				puts(convert(i,16));
				break;
			case 'd':
				i = va_arg(arg,int);
				if(i<0) {
					i = -i;
					putc('-');
				}
				puts(convert(i,10));
				break;
		}
	}
}