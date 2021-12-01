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


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;

static lv_style_t style_box;

static lv_obj_t * btn_start;
//static lv_obj_t * btn_stop;

#define SENSORS_QUANTITY   5
static lv_obj_t * ta_sensor[SENSORS_QUANTITY * 5];

extern void test_panel_update_button(int state);
#if LVGL_VERSION_MAJOR == 7
static void btn_event_cb(lv_obj_t *btn, lv_event_t e) {

	if (e == LV_EVENT_PRESSED) {
		if (btn_start == btn) {
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

#else
static inline void btn_event_cb(lv_event_t *e) {
	lv_event_code_t code = lv_event_get_code(e);

	if (code == LV_EVENT_PRESSED) {
		lv_obj_t *btn;
		int state;

		btn = lv_event_get_target(e);
		state = lv_obj_get_state(btn);

		if (state == LV_STATE_PRESSED) {
			if (btn_start == btn) {
				test_panel_update_button(1);
			}
			else {
				test_panel_update_button(0);
			}
		}
	}
}
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_widgets(void) {
#if LVGL_VERSION_MAJOR == 7
	tv = lv_tabview_create(lv_scr_act(), NULL);
#else
	tv = lv_tabview_create(lv_scr_act(),LV_DIR_TOP, LV_DPX(45));
#endif
	t1 = lv_tabview_add_tab(tv, "Basic");

	lv_style_init(&style_box);
#if LVGL_VERSION_MAJOR == 7
	lv_style_set_value_align(&style_box, LV_STATE_DEFAULT,
			LV_ALIGN_OUT_TOP_LEFT);
	lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, -LV_DPX(10));
	lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));
#else
	lv_style_set_text_align(&style_box, LV_ALIGN_OUT_TOP_LEFT);
#endif
	controls_create(t1);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static inline const char *get_sensor_def_str(int i) {

	switch(i) {
	case 0:
		return "Sen  1";
	case 1:
		return "Sen  2";
	case 2:
		return "Sen  3";
	case 3:
		return "Sen  4";
	case 4:
		return "Sen  5";
	case 5:
		return "Sen  6";
	case 6:
		return "Sen  7";
	case 7:
		return "Sen  8";
	case 8:
		return "Sen  9";
	case 9:
		return "Sen 10";
	case 10:
		return "Sen 11";
	case 11:
		return "Sen 12";
	case 12:
		return "Sen 13";
	case 13:
		return "Sen 14";
	case 14:
		return "Sen 15";
	case 15:
		return "Sen 16";
	case 16:
		return "Sen 17";
	case 17:
		return "Sen 18";
	case 18:
		return "Sen 19";
	case 19:
		return "Sen 20";
	case 20:
		return "Sen 21";
	case 21:
		return "Sen 22";
	case 22:
		return "Sen 23";
	case 23:
		return "Sen 24";
	case 24:
		return "Sen 25";
	case 25:
		return "Sen 26";
	}

	return "Sen  1";
}

static void controls_create(lv_obj_t *parent) {
#if LVGL_VERSION_MAJOR == 7
	lv_obj_t *label;
	lv_disp_size_t disp_size;
	lv_coord_t grid_w;
	lv_obj_t *h;
	lv_coord_t width;
//	lv_coord_t height;

	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent,
			disp_size <= LV_DISP_SIZE_SMALL ? 1 : 1, 1);
/***************/
	h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);


	lv_obj_set_drag_parent(h, true);

	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT,
			"Control");

	lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(h, grid_w);

	width = lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);
	btn_start = lv_btn_create(h, NULL);
	lv_btn_set_fit2(btn_start, LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(btn_start, width);

	label = lv_label_create(btn_start, NULL);
	lv_label_set_text(label, "Start");
	lv_obj_set_event_cb(btn_start, btn_event_cb);

	btn_stop = lv_btn_create(h, btn_start);
	label = lv_label_create(btn_stop, NULL);
	lv_label_set_text(label, "Stop");
	lv_obj_set_event_cb(btn_stop, btn_event_cb);

/**********************/
	h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(h, true);

	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT,
			"Sensors");

	lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(h, grid_w);
	lv_obj_set_height(h, lv_obj_get_height_grid(h, 2, 1));

	width = lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 5, 1);
	
	ta_sensor[0] = lv_textarea_create(h, NULL);
	lv_btn_set_fit2(ta_sensor[0], LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(ta_sensor[0], width);
	lv_textarea_set_text(ta_sensor[0], "");
	lv_textarea_set_placeholder_text(ta_sensor[0], get_sensor_def_str(0));
	lv_textarea_set_one_line(ta_sensor[0], true);
	lv_textarea_set_cursor_hidden(ta_sensor[0], true);
	lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

	ta_sensor[1] = lv_textarea_create(h, ta_sensor[0]);
	lv_btn_set_fit2(ta_sensor[1], LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(ta_sensor[1], width);
	lv_textarea_set_text(ta_sensor[1], "");
	lv_textarea_set_placeholder_text(ta_sensor[1], get_sensor_def_str(1));
	lv_textarea_set_one_line(ta_sensor[1], true);
	lv_textarea_set_cursor_hidden(ta_sensor[1], true);
	lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

	ta_sensor[2] = lv_textarea_create(h, ta_sensor[1]);
	lv_btn_set_fit2(ta_sensor[2], LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(ta_sensor[2], width);
	lv_textarea_set_text(ta_sensor[2], "");
	lv_textarea_set_placeholder_text(ta_sensor[2], get_sensor_def_str(2));
	lv_textarea_set_one_line(ta_sensor[2], true);
	lv_textarea_set_cursor_hidden(ta_sensor[2], true);
	lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

	ta_sensor[3] = lv_textarea_create(h, ta_sensor[2]);
	lv_btn_set_fit2(ta_sensor[3], LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(ta_sensor[3], width);
	lv_textarea_set_text(ta_sensor[3], "");
	lv_textarea_set_placeholder_text(ta_sensor[3], get_sensor_def_str(3));
	lv_textarea_set_one_line(ta_sensor[3], true);
	lv_textarea_set_cursor_hidden(ta_sensor[3], true);
	lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

	ta_sensor[4] = lv_textarea_create(h, ta_sensor[3]);
	lv_btn_set_fit2(ta_sensor[4], LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_set_width(ta_sensor[4], width);
	lv_textarea_set_text(ta_sensor[4], "");
	lv_textarea_set_placeholder_text(ta_sensor[4], get_sensor_def_str(4));
	lv_textarea_set_one_line(ta_sensor[4], true);
	lv_textarea_set_cursor_hidden(ta_sensor[4], true);
	lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_CENTER, 0, 0);

	/*----*/
	int i = 0;
	int j = 1;
	for (j = 1; j < 5; j++) {
		i = 0;
		ta_sensor[5 * j + i] = lv_textarea_create(h, ta_sensor[5 * j + i - 1]);
		lv_btn_set_fit2(ta_sensor[5 * j + i], LV_FIT_NONE, LV_FIT_NONE);
		lv_obj_set_width(ta_sensor[5 * j + i], width);
		lv_textarea_set_text(ta_sensor[5 * j + i], "");
		lv_textarea_set_placeholder_text(ta_sensor[5 * j + i],
				get_sensor_def_str(5 * j + i));
		lv_textarea_set_one_line(ta_sensor[5 * j + i], true);
		lv_textarea_set_cursor_hidden(ta_sensor[5 * j + i], true);
		lv_obj_align(ta_sensor[0], NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);

		for (i = 1; i < 5; i++) {
			ta_sensor[5 * j + i] = lv_textarea_create(h,
					ta_sensor[5 * j + i - 1]);
			lv_btn_set_fit2(ta_sensor[5 * j + i], LV_FIT_NONE, LV_FIT_NONE);
			lv_obj_set_width(ta_sensor[5 * j + i], width);
			lv_textarea_set_text(ta_sensor[5 * j + i], "");
			lv_textarea_set_placeholder_text(ta_sensor[5 * j + i],
					get_sensor_def_str(5 * j + i));
			lv_textarea_set_one_line(ta_sensor[5 * j + i], true);
			lv_textarea_set_cursor_hidden(ta_sensor[5 * j + i], true);
			lv_obj_align(ta_sensor[5 * j], NULL, LV_ALIGN_CENTER, 0, 0);
		}
	}

	/*******************/
#else
	static lv_coord_t col_dsc[] = { (180), (180), (180), (180), (180),
			LV_GRID_TEMPLATE_LAST };
	static lv_coord_t row_dsc[] = { (50), (50), (50), (50), (50),
			LV_GRID_TEMPLATE_LAST };

	/*Create a container with grid*/
	lv_obj_t *cont = lv_obj_create(parent);
	lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
	lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
	lv_obj_set_size(cont, 1024, 768 - 60);
	lv_obj_center(cont);
	lv_obj_set_layout(cont, LV_LAYOUT_GRID);

//    lv_obj_t * label;

	int i;
	for (i = 0; i < 25; i++) {
		uint8_t col = i % 5;
		uint8_t row = i / 5;

		ta_sensor[i] = lv_textarea_create(cont);
		/*Stretch the cell horizontally and vertically too
		 *Set span to 1 to make the cell 1 column/row sized*/
		lv_obj_set_grid_cell(ta_sensor[i], LV_GRID_ALIGN_STRETCH, col, 1,
				LV_GRID_ALIGN_STRETCH, row, 1);

//        label = lv_label_create(obj);
//        lv_label_set_text_fmt(label, "c%d, r%d", col, row);
//        lv_obj_center(label);
		lv_textarea_set_text(ta_sensor[i], "");
		lv_textarea_set_placeholder_text(ta_sensor[i], get_sensor_def_str(i));
		lv_textarea_set_one_line(ta_sensor[i], true);
		//lv_textarea_set_cursor_hidden(obj, true);
	}
#endif
}

void test_panel_set_sensor(int value[]) {
	int i;

	for (i = 0; i < SENSORS_QUANTITY; i++) {
		char main_buff[8];
		char rest_buff[8];
		char text_buff[18];

		itoa(abs(value[i]) / 100, main_buff, 10);
		if ((abs(value[i]) % 100) < 10) {
			rest_buff[0] = '0';
			itoa(abs(value[i]) % 100, rest_buff + 1, 10);
		} else {
			itoa(abs(value[i]) % 100, rest_buff, 10);
		}

		sprintf(text_buff, "%c%s.%s", (value[i] < 0) ? '-' : ' ', main_buff,
				rest_buff);
		lv_textarea_set_text(ta_sensor[i], text_buff);
	}
}
