#include "klipper.h"

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
