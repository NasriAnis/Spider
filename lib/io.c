#include <io.h>
#include <int.h>
#include <stdarg.h>
#include <kernel/console.h>
#include <stdint.h>

static char *convert(uintptr_t num, int base) {
    static const char digits[] = "0123456789ABCDEF";
    static char buffer[65];
    char *ptr = &buffer[sizeof(buffer) - 1];
    *ptr = '\0';

    do {
        *--ptr = digits[num % base];
        num /= base;
    } while (num != 0);

    return ptr;
}

void printf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);

    for (const char *p = format; *p != '\0'; p++) {
        if (*p != '%') {
            putc(*p);
            continue;
        }

        p++;
        if (*p == '\0') break;          // lone '%' at the end

        switch (*p) {
            case 's': {
                char *s = va_arg(arg, char *);
                puts(s ? s : "(null)");
                break;
            }
            case 'c':
                putc((char)va_arg(arg, int));
                break;
            case 'x':
                puts("0x");
                puts(convert(va_arg(arg, unsigned int), 16));
                break;
            case 'u':
                puts(convert(va_arg(arg, unsigned int), 10));
                break;
            case 'd': {
                int n = va_arg(arg, int);
                unsigned int u = (unsigned int)n;
                if (n < 0) {
                    putc('-');
                    u = -u;             // safe for INT_MIN
                }
                puts(convert(u, 10));
                break;
            }
            case 'p':
                puts("0x");
                puts(convert((uintptr_t)va_arg(arg, void *), 16));
                break;
            case '%':
                putc('%');
                break;
            default:                    // unknown: print as-is
                putc('%');
                putc(*p);
                break;
        }
    }

    va_end(arg);
}