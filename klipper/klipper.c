#include "klipper.h"


static void drag_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    lv_indev_t * indev = lv_indev_get_act();
    if(indev == NULL)  return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    if(x>1024) x = 1024;
    if(x<0) x = 0;
    if(y<0) y = 0;
    if(y>600) y = 600;

    lv_obj_set_pos(obj, x, y);
}


/**
 * Make an object dragable.
 */
void example_obj_2(void)
{
    lv_obj_t * obj;
    obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, 150, 100);
    lv_obj_add_event_cb(obj, drag_event_handler, LV_EVENT_PRESSING, NULL);
    lv_obj_set_align(obj, LV_ALIGN_TOP_MID);

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, "Drag me");
    // lv_obj_center(label);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);

}

void klipper_main_func(void)
{
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello klipper!");
    lv_obj_set_pos(label, 100, 10);

    lv_obj_t * sw = lv_switch_create(lv_scr_act());
    lv_obj_center(sw);
    lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(sw, sw_event, LV_EVENT_VALUE_CHANGED, label);
}
