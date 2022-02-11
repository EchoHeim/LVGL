#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "fontadpt.h"
//author:	mala
//usage:	adapt lvgl5 Chinese font to lvgl6

const uint8_t * adapt_get_bitmap_sparse(const lv_font_t * font, uint32_t unicode_letter)
{
	font_adpt_t *fa = font->user_data;
    /*Check the range*/
    if(unicode_letter < fa->uniFirst || unicode_letter > fa->uniLast) return NULL;

    uint32_t i;
    for(i = 0; fa->unicodeList[i] != 0; i++) {
        if(fa->unicodeList[i] == unicode_letter) {
            return &fa->bitmap[fa->desc[i].glyph_index];
        }
    }

    return NULL;
}

int16_t lv_font_get_width_sparse(font_adpt_t *fa, uint32_t unicode_letter)
{
    /*Check the range*/
    if(unicode_letter < fa->uniFirst || unicode_letter > fa->uniLast) return -1;

    uint32_t i;
    for(i = 0; fa->unicodeList[i] != 0; i++) {
        if(fa->unicodeList[i] == unicode_letter) {
            return fa->desc[i].w_px;
        }
    }

    return -1;
}

bool adapt_get_glyph_dsc_cb(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
    /*Your code here*/
    uint32_t i;
	font_adpt_t *fa = font->user_data;
    /*Check the range*/
    if(unicode_letter < fa->uniFirst || unicode_letter > fa->uniLast) return false;  
    for(i = 0; fa->unicodeList[i] != 0; i++) {
        if(fa->unicodeList[i] == unicode_letter) {
            break;
        }
    }
    if(fa->unicodeList[i]==0)
		return false;

    dsc_out->adv_w = fa->fontWidth + fa->defGap;        /*Horizontal space required by the glyph in [px]*/
    dsc_out->box_h = fa->fontHeight;         /*Height of the bitmap in [px]*/
    dsc_out->box_w = lv_font_get_width_sparse(fa, unicode_letter);         /*Width of the bitmap in [px]*/
    dsc_out->ofs_x = 0;         /*X offset of the bitmap in [pf]*/
    dsc_out->ofs_y = 0;         /*Y offset of the bitmap measured from the as line*/
    dsc_out->bpp   = 1;         /*Bit per pixel: 1/2/4/8*/    
    
    return true;                /*true: glyph found; false: glyph was not found*/
}

font_adpt_t* init_adpt( uint8_t w, uint8_t h, uint8_t g, uint32_t f, uint32_t l,	
		const uint32_t *ul, const lv_font_glyph_dsc_old_t *pd, const uint8_t* bmp, char* name)
{
	font_adpt_t *afont = (font_adpt_t*) malloc(sizeof(font_adpt_t));
	afont->fontWidth = w;
	afont->fontHeight = h;
	afont->defGap = g;
	afont->unicodeList = ul;
	afont->desc = pd;
	afont->bitmap = bmp;
	afont->uniFirst = f;
	afont->uniLast = l;
	strncpy(afont->name, name, 20);
	return afont;
}

void attach_font(lv_font_t* font, font_adpt_t* a)
{
	font->user_data = (void*)a;
	font->get_glyph_dsc = adapt_get_glyph_dsc_cb;           /*Set a callback to get info about gylphs*/
	font->get_glyph_bitmap = adapt_get_bitmap_sparse;       /*Set a callback to get bitmap of a glyp*/
	font->line_height = a->fontHeight+1;                    /*The real line height where any text fits*/
	font->base_line = 4; 
}

void free_adapt(lv_font_t* font)
{
	font_adpt_t* p=(font_adpt_t*) font->user_data;
	if(p)
		free(p);
}
