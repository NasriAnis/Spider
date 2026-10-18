#include "../lib/int.h"
#include "font.h"

extern char _binary_default8x16_psfu_start;
extern char _binary_default8x16_psfu_end;

const uint8_t *psf_get_glyph(uint32_t c)
{
	PSF_font *font = (PSF_font *)&_binary_default8x16_psfu_start;

	if (c >= font->numglyph)
		c = 0;

	return (const uint8_t *)&_binary_default8x16_psfu_start
		+ font->headersize
		+ c * font->bytesperglyph;
}