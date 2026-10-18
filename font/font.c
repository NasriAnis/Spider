#include "../lib/int.h"
#include "font.h"

extern char _binary_default8x16_psfu_start;
extern char _binary_default8x16_psfu_end;

const uint8_t *psf_get_glyph(uint32_t c)
{
    PSF_font *font = (PSF_font *)&_binary_default8x16_psfu_start;
    uint32_t idx = c;

    if (idx >= font->numglyph)
        idx = 0;

    return (const uint8_t *)&_binary_default8x16_psfu_start
        + font->headersize
        + idx * font->bytesperglyph;
}