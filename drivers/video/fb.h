#include <int.h>

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

#define BG_COLOR 0x00000000
#define RED 0x00FF0000
#define WHITE 0x00FFFFFF

void init_fb_driver(uintptr_t addr, uint32_t width, uint32_t height, uint32_t pitch);