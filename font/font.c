#include "../lib/int.h"
#include "font.h"

extern char _binary_default8x16_psfu_start;
extern char _binary_default8x16_psfu_end;

static uint16_t unicode_table[0x10000];
uint16_t *unicode;

// TODO check for magic number

void psf_init()
{
	uint16_t glyph = 0;
	/* cast the address to PSF header struct */
	PSF_font *font = (PSF_font*)&_binary_default8x16_psfu_start;
	/* is there a unicode table? */
	if (font->flags == 0) {
		unicode = 0;
		return;
	}

	/* get the offset of the table */
	unsigned char *s = (unsigned char *)(
		(unsigned char*)&_binary_default8x16_psfu_start +
		font->headersize +
		font->numglyph * font->bytesperglyph
	);
	/* use the static translation table instead of calloc */
	unicode = unicode_table;
	while(s < (unsigned char*)&_binary_default8x16_psfu_end) {
		uint16_t uc = (uint16_t)s[0];
		if(uc == 0xFF) {
			glyph++;
			s++;
			continue;
		} else if(uc & 128) {
			/* UTF-8 to unicode */
			if((uc & 32) == 0 ) {
				uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
				s++;
			} else
			if((uc & 16) == 0 ) {
				uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
				s+=2;
			} else
			if((uc & 8) == 0 ) {
				uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
				s+=3;
			} else
				uc = 0;
		}
		/* save translation */
		unicode[uc] = glyph;
		s++;
	}
}

const uint8_t *psf_get_glyph(uint32_t c)
{
	PSF_font *font = (PSF_font *)&_binary_default8x16_psfu_start;
	uint32_t idx = c;

	if (unicode && c < 0x10000)
		idx = unicode[c];
	if (idx >= font->numglyph)
		idx = 0;

	return (const uint8_t *)&_binary_default8x16_psfu_start
		 + font->headersize
		 + idx * font->bytesperglyph;
}