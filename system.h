#ifndef __SYSTEM_H__
#define	__SYSTEM_H__

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#include "lvgl/lvgl.h"
#include "lv_conf.h"

#ifdef ON_Embedded	  //  on embed if defined

#include "lv_drivers/indev/evdev.h"
#include "lv_drivers/display/fbdev.h"

#else     //on pc simulator 

#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"

#endif

// global var
extern lv_indev_t * key_indev;		//keyboard or keypad 
extern lv_indev_t * mouse_indev ;	//mouse or touchscreen
extern lv_font_t myfont;

// global func
void init_cn_font();
void ui_main();
bool embpad_read(lv_indev_drv_t* pdev, lv_indev_data_t*data);

uint32_t kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0x4000003a,	/* 59 - F1 key ... > */
    0x4000003b,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    17,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    20,	/* Left Arrow */
    0,
    19,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    18,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* 89 All other keys are undefined */
    0,0,0,0,0,0,0,0,0,0, /*99*/
    0,0,0,
    17,	/* up */
    0,
    20,	/* left */
    19, /* right */
    0,
    18	/*down*/
};


#endif   //__SYSTEM_H__
