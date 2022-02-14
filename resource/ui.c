#include "lvgl/lvgl.h"
#include "system.h"

#define	LV_KEY_F1	0x4000003a
#define LV_KEY_F2	0x4000003b

static lv_group_t * grpMain, *grpCfg, *grpActive;
lv_style_t style_cn;
lv_obj_t *scrMain, *scrCfg, *scrActive;
static lv_obj_t * pageCfg;

static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_PRESSED) {
        lv_event_send(scrActive, LV_EVENT_KEY, LV_KEY_DOWN);
    }
 
}

static void group_focus_cb(lv_group_t * group)
{
    lv_obj_t * f = lv_group_get_focused(group);
    lv_page_focus(pageCfg, f, LV_ANIM_ON);
}

void proc_config_page(lv_obj_t *obj, lv_event_t event)
{
	
	if(LV_EVENT_KEY == event){
		uint32_t	theKey = lv_event_get_data();
		
		if(LV_KEY_DOWN==theKey){
			lv_group_set_editing(grpActive, false); /*Editing is not used by KEYPAD is be sure it is disabled*/
			lv_group_focus_next(grpActive);
		}
		else if(LV_KEY_UP==theKey){
			lv_group_set_editing(grpActive, false); /*Editing is not used by KEYPAD is be sure it is disabled*/
			lv_group_focus_prev(grpActive);			
		}
		else if(LV_KEY_ESC == theKey){
			lv_group_set_editing(grpActive, false); /*Editing is not used by KEYPAD is be sure it is disabled*/
			lv_obj_t* parent = obj->user_data;
			lv_scr_load(parent);
			scrActive = parent;
			lv_indev_set_group(key_indev, NULL);
		}
		else if(LV_KEY_RIGHT == theKey){
			
		}
	}
}

void proc_main_page(lv_obj_t *obj, lv_event_t event)
{	
	if(event == LV_EVENT_KEY && lv_event_get_data()== LV_KEY_F1){ //F2 
		lv_scr_load(scrCfg);
		grpActive = grpCfg;
		scrActive = scrCfg;
		lv_indev_set_group(key_indev, grpCfg);
	}
}

lv_obj_t* create_label(lv_coord_t x, lv_coord_t y, lv_obj_t* parent, lv_style_t *s, char* text)
{
	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_obj_set_pos(label, x, y);
	if(s){
		lv_obj_set_style(label, s);
	}
	lv_label_set_text(label, text);
	return label;
}

lv_obj_t* create_text_area(lv_coord_t x, lv_coord_t y, lv_obj_t* parent, lv_style_t *s, char* preText)
{
	lv_obj_t *ta = lv_ta_create(parent, NULL);
	lv_obj_set_pos(ta, x, y);
	if(s){
		lv_obj_set_style(ta, s);
	}
	if(preText){
		 lv_ta_set_text(ta, preText);
		 lv_ta_set_text_sel(ta, true);
	}
	return ta;
}

lv_obj_t* create_main_scr()
{
    grpMain = lv_group_create();
	
    lv_obj_t* scr = lv_obj_create(NULL, NULL); 
	
    lv_obj_t* label = create_label(10, 10, scr, &style_cn, "福莫");
 
    lv_obj_set_event_cb(scr,  proc_main_page);
    return scr;
}

lv_obj_t* create_config_scr()
{   
	char lb_txt[20];
	int i;
	grpCfg = lv_group_create();
	lv_obj_t *scr = lv_obj_create(NULL, NULL); 
	pageCfg = lv_page_create(scr, NULL);
	
	lv_obj_t *btn = lv_btn_create(pageCfg, NULL);
	lv_obj_set_pos(btn, 30, 180);
	lv_obj_set_size(btn, 25,25);
	lv_obj_set_event_cb(btn, btn_event_cb);
	
    lv_obj_set_size(pageCfg, 320, 240);
	for(i=0; i<6; i++){
		memset(lb_txt, 0, 20);
		sprintf(lb_txt, "param%d", i+1);
		lv_obj_t* label = create_label(10, 50*i+10, pageCfg, NULL, lb_txt);
		lv_obj_t *txt = create_text_area(100, 50*i+10, pageCfg, NULL, "");
		lv_ta_set_one_line(txt, true);
		lv_group_add_obj(grpCfg,  txt);
	}
	
	lv_group_set_focus_cb(grpCfg, group_focus_cb);
	lv_obj_set_event_cb(scr,  proc_config_page);
	return scr;
}
	
void ui_main()
{
//mala: create style with chinese font
    lv_style_copy(&style_cn, &lv_style_plain);
    style_cn.text.font = &myfont;
    
//mala:create main & config screen, key F2 & ESC switch them
    scrMain = create_main_scr();
    scrCfg = create_config_scr();

	scrCfg->user_data = scrMain;
	scrActive = scrMain;
	lv_scr_load(scrMain);

}
 

//dont forget to move following func to __SYSTEM.c
/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
bool embpad_read(lv_indev_drv_t *pdev, lv_indev_data_t*data)
{
	
	struct input_event ev;
	if (read(fdKey, &ev, sizeof (struct input_event)) < sizeof (struct input_event)) {
		data->key = 0; data->state=0;
	}
	else{
		if(ev.type==1 && ev.code<128){
			data->key = kbdus[ev.code];
			data->state = (ev.value>0);
	//		printf("read kb:%d %d\n", data->key, data->state);
		}
	}

	return false; //No buffering so no more data read
}

//mala: this function will be called to check keyboard input periodically
bool ui_keyboard_read(lv_indev_drv_t *indev, lv_indev_data_t*data)
{
	static lv_indev_data_t ki;
	uint32_t keyPressed = 0;

	embpad_read(NULL, data);

	//custom check
	if(ki.state==0 && data->state==1){	//a key is pressed
		memcpy(&ki, data, sizeof(ki));
		keyPressed = data->key;
	}
	else if(ki.state==1 && data->state==1){
		ki.key=0;
	}
	else if(ki.state==1 && data->state==0){ //the key is released
		memset(&ki, 0, sizeof(ki));
		
	}
	
	switch(keyPressed){
		case LV_KEY_F1:	//F1
		case LV_KEY_F2:	//F2
		case LV_KEY_ESC:
		case LV_KEY_UP:
		case LV_KEY_DOWN:	
		lv_indev_wait_release(key_indev);
			lv_event_send(scrActive, LV_EVENT_KEY, keyPressed);
			
			break;
		default: break;
		
	}
	
	return false; //No buffering so no more data read
}
