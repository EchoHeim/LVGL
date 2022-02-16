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
#define DISP_BUF_SIZE (128 * LV_HOR_RES)        /*display buffer*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_disp_draw_buf_t disp_buf;

static lv_color_t disp_buf_1[DISP_BUF_SIZE];    //double buffer
static lv_color_t disp_buf_2[DISP_BUF_SIZE];

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char ** argv)
{
    printf("Welcome to LVGL!\n"); 
    lv_init();      /*Initialize LittlevGL*/

	lv_disp_draw_buf_init(&disp_buf, disp_buf_1, disp_buf_2, DISP_BUF_SIZE);
	
    hal_init();     /*Initialize the HAL (display, input devices, tick) for LittlevGL*/

	// init_cn_font();

    /******************  Load a demo  ******************/

    // lv_demo_music();
    // lv_demo_widgets();
    // lv_demo_benchmark();
    // klipper_main_func();
    lv_example_obj_2();

	while(1) {
        /* Periodically call the lv_task handler. It could be done in a timer interrupt or an OS task too.*/
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
//--------------- display driver ---------------//
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv); /*Basic initialization*/

#ifdef ON_Embedded
	fbdev_init();		    //framebuffer driver at real env
	disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = LV_HOR_RES;
    disp_drv.ver_res = LV_VER_RES;
    disp_drv.flush_cb = fbdev_flush;
    lv_disp_drv_register(&disp_drv);
#endif

#ifdef PC_MONITOR
	sdl_init();
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    disp_drv.flush_cb = monitor_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.antialiasing = 1;

    SDL_CreateThread(tick_thread, "tick", NULL);

    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
#endif

//--------------- mouse/touchscreen driver ---------------//
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;

#ifdef ON_Embedded
    evdev_init();	//touch pad at /dev/input/event0
    indev_drv.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv);
#endif

#ifdef PC_MONITOR
    // mouse_init();
    indev_drv.read_cb = sdl_mouse_read;         //real pc mouse
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv);

    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act());        /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);             /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);               /*Connect the image  object to the driver*/
#endif

//--------------- keyboard/mousewheel driver ---------------//
#ifdef PC_MONITOR
    // keyboard_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);    /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_1.read_cb = sdl_keyboard_read;
    lv_indev_t *keyboard_indev = lv_indev_drv_register(&indev_drv_1);
    lv_indev_set_group(keyboard_indev, g);

    // mousewheel_init();
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);    /*Basic initialization*/
    indev_drv_2.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_2.read_cb = sdl_mousewheel_read;
    lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(enc_indev, g);
#endif

//--------------- keyboard for Embedded  driver ---------------//
#ifdef ON_Embedded
#if 0
    int fdKey = open ("/dev/input/event1", O_RDONLY);
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
    lv_indev_t *key_indev = lv_indev_drv_register(&key_drv);
#endif
#endif
}

#if LV_TICK_CUSTOM
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
#endif

#ifdef PC_MONITOR
/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data) {
    (void)data;

    while(1) {
        SDL_Delay(5);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}
#endif 
