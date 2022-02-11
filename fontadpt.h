#ifndef __FONTADPT_H__
#define __FONTADPT_H__
//mala this file is a temp tool for lvgl6 to adapt to lvgl5

struct lv_font_t;
typedef struct
{
    uint32_t w_px         :8;
    uint32_t glyph_index  :24;

}lv_font_glyph_dsc_old_t;

typedef struct{
	uint8_t fontWidth;
	uint8_t fontHeight;
	uint8_t defGap;
	uint32_t *unicodeList;
	uint32_t	uniFirst, uniLast;
	lv_font_glyph_dsc_old_t  *desc;
	uint8_t *bitmap;
	char	name[20];
}font_adpt_t;

font_adpt_t* init_adpt( uint8_t w, uint8_t h, uint8_t g, uint32_t f, uint32_t l,
		const uint32_t *ul, const lv_font_glyph_dsc_old_t *pd, const uint8_t* bmp, char* name);

void attach_font(lv_font_t* font, font_adpt_t* a);

#endif
