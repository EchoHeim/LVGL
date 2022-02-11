/**
 * @file main
 */

/*********************
 *      INCLUDES
 *********************/
#include "system.h"

#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/
/*display buffer*/

#define DISP_BUF_SIZE (128 * LV_HOR_RES)

static lv_disp_draw_buf_t disp_buf;

static lv_color_t disp_buf_1[DISP_BUF_SIZE];	    //double buffer
static lv_color_t disp_buf_2[DISP_BUF_SIZE];

lv_indev_t *key_indev;		//keyboard or keypad 
lv_indev_t *mouse_indev ;	//mouse or touchscreen

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void * data);
bool ui_keyboard_read(lv_indev_drv_t *indev, lv_indev_data_t*data);
int fdKey;

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char ** argv)
{
    /*Initialize LittlevGL*/
    printf("Welcome to LVGL!\n"); 
    lv_init();

	lv_disp_draw_buf_init(&disp_buf, disp_buf_1, disp_buf_2, DISP_BUF_SIZE);
	
    /*Initialize the HAL (display, input devices, tick) for LittlevGL*/
    hal_init();

	init_cn_font();

    /*Load a demo*/
//    ui_main();  our real prog

    // lv_demo_music();
    // lv_demo_stress();
    lv_demo_widgets();

    /* A keyboard and encoder (mouse wheel) control example*/

	while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(5 * 1000);       /*Just to let the system breath*/
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void)
{
////////display driver
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);

#ifdef	ON_Embedded
	fbdev_init();		//framebuffer driver at real env
	disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = LV_HOR_RES;
    disp_drv.ver_res = LV_VER_RES;
    disp_drv.flush_cb = fbdev_flush;

#else
	//pc monitor
	monitor_init();
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = monitor_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
#endif

    lv_disp_drv_register(&disp_drv);
	
/////////mouse/touchscreen driver//////////////
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
#ifdef ON_Embedded
    evdev_init();	//touch pad at /dev/input/event0
    indev_drv.read_cb = evdev_read;
    
#else
    mouse_init();
    indev_drv.read_cb = mouse_read;         //real pc mouse

#endif

    mouse_indev = lv_indev_drv_register(&indev_drv);

#ifndef ON_Embedded
    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon);                          /*Declare the image file.*/
    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);             /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);               /*Connect the image  object to the driver*/
#endif

/////////////////keypad/////////////////////////
#if 0	
	fdKey = open ("/dev/input/event1", O_RDONLY);
	if (fdKey == -1) {
		printf("can not open /dev/input/event1\n");
		exit(1);
	}
	
	int flags = fcntl(fdKey, F_GETFL, 0);
	if(fcntl(fdKey, F_SETFL, flags | O_NONBLOCK)){
		printf("can not set keyboard device in nonblocking mode\n");
		exit(1);
	}
	
    lv_indev_drv_t  key_drv;
    lv_indev_drv_init(&key_drv);
    key_drv.type = LV_INDEV_TYPE_KEYPAD;
   // key_drv.read_cb = ui_keyboard_read;	//real keyboard
    key_indev = lv_indev_drv_register(&key_drv);
#endif
////////////////////keypad///////////////////////
}


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}



