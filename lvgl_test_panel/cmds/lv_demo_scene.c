/**
 * @file lv_demo_widgets.c
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void controls_create(lv_obj_t * parent);
static void visuals_create(lv_obj_t * parent);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;

static lv_style_t style_box;

static lv_obj_t * btn_start;
static lv_obj_t * btn_stop;
static lv_obj_t *switch_state;

static lv_obj_t * ta_sensor[5];
static lv_obj_t * bar_sensor[5];

extern void test_panel_update_button(int state) ;
static void btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_PRESSED) {
        if(btn_start == btn) {
            if (lv_btn_get_state(btn) == LV_BTN_STATE_PRESSED) {
            	test_panel_update_button(1);
            }
        } else {
            if (lv_btn_get_state(btn) == LV_BTN_STATE_PRESSED) {
            	test_panel_update_button(0);
            }
        }

    }

}

static void switch_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
    	test_panel_update_button(lv_switch_get_state(obj));
    }
}

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_widgets(void)
{
    tv = lv_tabview_create(lv_scr_act(), NULL);

    t1 = lv_tabview_add_tab(tv, "Basic");
    t2 = lv_tabview_add_tab(tv, "Visuals");

    lv_style_init(&style_box);
    lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
    lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, - LV_DPX(10));
    lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

    controls_create(t1);
    visuals_create(t2);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void controls_create(lv_obj_t * parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 1, 1);

    lv_obj_t * h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Control");

    lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);

    btn_start = lv_btn_create(h, NULL);
    lv_btn_set_fit2(btn_start, LV_FIT_NONE, LV_FIT_NONE);
    lv_obj_set_width(btn_start, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
    lv_obj_t * label = lv_label_create(btn_start, NULL);
    lv_label_set_text(label ,"Start");
    lv_obj_set_event_cb(btn_start, btn_event_cb);

    btn_stop = lv_btn_create(h, btn_start);
    label = lv_label_create(btn_stop, NULL);
    lv_label_set_text(label ,"Stop");
    lv_obj_set_event_cb(btn_stop, btn_event_cb);

    h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);

    lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Sensors");

    lv_cont_set_fit2(h, LV_FIT_NONE,  LV_FIT_TIGHT);
    lv_obj_set_width(h, grid_w);
    lv_obj_set_height(h, lv_obj_get_height_grid(h, 2, 1));


    ta_sensor[0] = lv_textarea_create(h, NULL);
    lv_btn_set_fit2(ta_sensor[0], LV_FIT_NONE, LV_FIT_NONE);
    lv_obj_set_width(ta_sensor[0], lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 5, 1));

    lv_textarea_set_text(ta_sensor[0], "");
    lv_textarea_set_placeholder_text(ta_sensor[0], "Sen 1");
    lv_textarea_set_one_line(ta_sensor[0], true);
    lv_textarea_set_cursor_hidden(ta_sensor[0], true);

    ta_sensor[1] = lv_textarea_create(h, ta_sensor[0]);
    lv_textarea_set_text(ta_sensor[1], "");
    lv_textarea_set_placeholder_text(ta_sensor[1], "Sen 2");
    lv_textarea_set_one_line(ta_sensor[1], true);
    lv_textarea_set_cursor_hidden(ta_sensor[1], true);



    ta_sensor[2] = lv_textarea_create(h, ta_sensor[1]);
    lv_textarea_set_text(ta_sensor[2], "");
    lv_textarea_set_placeholder_text(ta_sensor[2], "Sen 3");
    lv_textarea_set_one_line(ta_sensor[2], true);
    lv_textarea_set_cursor_hidden(ta_sensor[2], true);

    ta_sensor[3] = lv_textarea_create(h, ta_sensor[2]);
    lv_textarea_set_text(ta_sensor[3], "");
    lv_textarea_set_placeholder_text(ta_sensor[3], "Sen 4");
    lv_textarea_set_one_line(ta_sensor[3], true);
    lv_textarea_set_cursor_hidden(ta_sensor[3], true);

    ta_sensor[4] = lv_textarea_create(h, ta_sensor[3]);
    lv_textarea_set_text(ta_sensor[4], "");
    lv_textarea_set_placeholder_text(ta_sensor[4], "Sen 5");
    lv_textarea_set_one_line(ta_sensor[4], true);

    lv_textarea_set_cursor_hidden(ta_sensor[4], true);

    lv_obj_t * ta_label;

    ta_label = lv_label_create(h, NULL);
    lv_label_set_text(ta_label, "Sen 1");
    lv_obj_align(ta_label, ta_sensor[0], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    ta_label = lv_label_create(h, NULL);
    lv_label_set_text(ta_label, "Sen 2");
    lv_obj_align(ta_label, ta_sensor[1], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    ta_label = lv_label_create(h, NULL);
    lv_label_set_text(ta_label, "Sen 3");
    lv_obj_align(ta_label, ta_sensor[2], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    ta_label = lv_label_create(h, NULL);
    lv_label_set_text(ta_label, "Sen 4");
    lv_obj_align(ta_label, ta_sensor[3], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    ta_label = lv_label_create(h, NULL);
    lv_label_set_text(ta_label, "Sen 5");
    lv_obj_align(ta_label, ta_sensor[4], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

}

static void visuals_create(lv_obj_t * parent)
{
	lv_coord_t grid_h;
	lv_coord_t grid_w;
//	lv_obj_t * label= NULL;

    lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

 //   lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);


    /*Create a bar and use the backgrounds value style property to display the current value*/
    lv_obj_t * bar_h = lv_cont_create(parent, NULL);
    lv_cont_set_fit2(bar_h, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_add_style(bar_h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_style_local_value_str(bar_h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Sensors");

    grid_w = lv_page_get_width_grid(parent, 2, 1);
    grid_h = lv_page_get_height_grid(parent, 5, 1);


    lv_obj_set_width(bar_h, grid_w);
    lv_obj_set_height(bar_h, lv_page_get_height_grid(parent, 2, 1));


    bar_sensor[0] = lv_bar_create(bar_h, NULL);
    lv_obj_set_width(bar_sensor[0], lv_obj_get_width_fit(bar_h));
    lv_obj_set_style_local_value_font(bar_sensor[0], LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(bar_sensor[0], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(bar_sensor[0], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
    lv_obj_set_style_local_margin_bottom(bar_sensor[0], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
    lv_obj_set_y(bar_sensor[0], (LV_DPI / 20 + grid_h) * 0);
    lv_obj_align(bar_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

    bar_sensor[1] = lv_bar_create(bar_h, bar_sensor[0]);
    lv_obj_set_width(bar_sensor[1], lv_obj_get_width_fit(bar_h));
    lv_obj_set_style_local_value_font(bar_sensor[1], LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(bar_sensor[1], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(bar_sensor[1], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
    lv_obj_set_style_local_margin_bottom(bar_sensor[1], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
    lv_obj_set_y(bar_sensor[1], (LV_DPI / 20 + grid_h) * 1);
    lv_obj_align(bar_sensor[1], NULL, LV_ALIGN_CENTER, 0, 0);

    bar_sensor[2] = lv_bar_create(bar_h, bar_sensor[1]);
    lv_obj_set_width(bar_sensor[2], lv_obj_get_width_fit(bar_h));
    lv_obj_set_style_local_value_font(bar_sensor[2], LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(bar_sensor[2], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(bar_sensor[2], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
    lv_obj_set_style_local_margin_bottom(bar_sensor[2], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
    lv_obj_set_y(bar_sensor[2], (LV_DPI / 20 + grid_h) * 2);
    lv_obj_align(bar_sensor[2], NULL, LV_ALIGN_CENTER, 0, 0);

    bar_sensor[3] = lv_bar_create(bar_h, bar_sensor[2]);
    lv_obj_set_width(bar_sensor[3], lv_obj_get_width_fit(bar_h));
    lv_obj_set_style_local_value_font(bar_sensor[3], LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(bar_sensor[3], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(bar_sensor[3], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
    lv_obj_set_style_local_margin_bottom(bar_sensor[3], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
    lv_obj_set_y(bar_sensor[3], (LV_DPI / 20 + grid_h) * 3);
    lv_obj_align(bar_sensor[3], NULL, LV_ALIGN_CENTER, 0, 0);

    bar_sensor[4] = lv_bar_create(bar_h, bar_sensor[3]);
    lv_obj_set_width(bar_sensor[4], lv_obj_get_width_fit(bar_h));
    lv_obj_set_style_local_value_font(bar_sensor[4], LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(bar_sensor[4], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(bar_sensor[4], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
    lv_obj_set_style_local_margin_bottom(bar_sensor[4], LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
    lv_obj_set_y(bar_sensor[4], (LV_DPI / 20 + grid_h) * 4);
    lv_obj_align(bar_sensor[4], NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * h = lv_cont_create(parent, NULL);
    lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
    lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_drag_parent(h, true);
    lv_obj_set_width(h, lv_page_get_width_grid(parent, 2, 1));
    lv_obj_set_height(h, lv_page_get_height_grid(parent, 2, 1));
    lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Controls");

    switch_state = lv_switch_create(h, NULL);
    lv_obj_set_event_cb(switch_state, switch_event_handler);
}


void test_panel_set_sensor(int value[5]) {
	int i;
#if 1
	for (i = 0; i < 5; i ++) {
		char main_buff[8];
		char rest_buff[8];
		char text_buff[16];

		itoa(value[i] / 100, main_buff, 10);
		itoa(abs(value[i]) % 100, rest_buff, 10);
		sprintf(text_buff, "%s.%s", main_buff, rest_buff);
		lv_textarea_set_text(ta_sensor[i],text_buff);
		lv_bar_set_value(bar_sensor[i], value[i]/100, LV_ANIM_OFF);
	}
#else
	for (i = 0; i < 5; i ++) {
		char buff[10];
		lv_textarea_set_text(ta_sensor[i],itoa(value[i], buff, 10));
		lv_bar_set_value(bar_sensor[i], value[i], LV_ANIM_OFF);
	}
#endif
}
