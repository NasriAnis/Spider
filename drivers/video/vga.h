#define VGA_MEM   ((volatile unsigned char *)0xB8000)

#define CTRL_PORT 0x3D4
#define DATA_PORT 0x3D5

#define COLS      80
#define ROWS      25

#define ATTR_WHITE 0x0F
#define ATTR_RED   0x04