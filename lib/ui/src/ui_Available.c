// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: V0





#include "ui.h"


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void selectors_create(lv_obj_t * parent);
static void text_input_create(lv_obj_t * parent);
static void msgbox_create(void);


static void ta_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;

/**********************/



void ui_Available_screen_init(void)
{
    ui_Available = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Available, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image10 = lv_image_create(ui_Available);
    lv_image_set_src(ui_Image10, &ui_img_available_png);
    lv_obj_set_width(ui_Image10, LV_SIZE_CONTENT);   /// 240
    lv_obj_set_height(ui_Image10, LV_SIZE_CONTENT);    /// 240
    lv_obj_set_align(ui_Image10, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image10, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_remove_flag(ui_Image10, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_NextMeet = lv_textarea_create(ui_Available);
    lv_obj_set_width(ui_NextMeet, 143);
    lv_obj_set_height(ui_NextMeet, LV_SIZE_CONTENT);    /// 70
    lv_obj_set_x(ui_NextMeet, -15);
    lv_obj_set_y(ui_NextMeet, 67);
    lv_obj_set_align(ui_NextMeet, LV_ALIGN_CENTER);
    lv_textarea_set_max_length(ui_NextMeet, 6);
    lv_textarea_set_placeholder_text(ui_NextMeet, "00:00h");
    lv_textarea_set_one_line(ui_NextMeet, true);
    lv_obj_set_style_text_color(ui_NextMeet, lv_color_hex(0x3D85C6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_NextMeet, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_NextMeet, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_NextMeet, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_NextMeet, lv_color_hex(0xDEF2C5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_NextMeet, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_NextMeet, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_NextMeet, 0, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_set_style_text_color(ui_NextMeet, lv_color_hex(0x3D85C6), LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_NextMeet, 255, LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_NextMeet, LV_TEXT_ALIGN_CENTER, LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_NextMeet, &lv_font_montserrat_32, LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);

    //lv_obj_add_event_cb(ui_Available, ui_event_Available, LV_EVENT_ALL, NULL);
    tv = lv_tabview_create(lv_screen_active());

    t1 = lv_tabview_add_tab(tv, "Selectors");
    t2 = lv_tabview_add_tab(tv, "Text input");

    //selectors_create(t1);
    //text_input_create(t2);

    msgbox_create();
}

static void msgbox_create(void)
{
    lv_obj_t * mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Hi");
    lv_msgbox_add_text(mbox, "Welcome to the keyboard and encoder demo");

    lv_obj_t * btn = lv_msgbox_add_footer_button(mbox, "Ok");
    lv_obj_add_event_cb(btn, msgbox_event_cb, LV_EVENT_CLICKED, mbox);
    lv_group_focus_obj(btn);
    lv_obj_add_state(btn, LV_STATE_FOCUS_KEY);
    lv_group_focus_freeze(lv_group_create(), true);

    lv_obj_align(mbox, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * bg = lv_obj_get_parent(mbox);
    lv_obj_set_style_bg_opa(bg, LV_OPA_70, 0);
    lv_obj_set_style_bg_color(bg, lv_palette_main(LV_PALETTE_GREY), 0);
    
}

static void selectors_create(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * obj;

    obj = lv_table_create(parent);
    lv_table_set_cell_value(obj, 0, 0, "00");
    lv_table_set_cell_value(obj, 0, 1, "01");
    lv_table_set_cell_value(obj, 1, 0, "10");
    lv_table_set_cell_value(obj, 1, 1, "11");
    lv_table_set_cell_value(obj, 2, 0, "20");
    lv_table_set_cell_value(obj, 2, 1, "21");
    lv_table_set_cell_value(obj, 3, 0, "30");
    lv_table_set_cell_value(obj, 3, 1, "31");
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_calendar_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_buttonmatrix_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_checkbox_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_slider_create(parent);
    lv_slider_set_range(obj, 0, 10);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_switch_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_spinbox_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_dropdown_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    obj = lv_roller_create(parent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t * list = lv_list_create(parent);
    lv_obj_update_layout(list);
    if(lv_obj_get_height(list) > lv_obj_get_content_height(parent)) {
        lv_obj_set_height(list, lv_obj_get_content_height(parent));
    }

    lv_list_add_button(list, LV_SYMBOL_OK, "Apply");
    lv_list_add_button(list, LV_SYMBOL_CLOSE, "Close");
    lv_list_add_button(list, LV_SYMBOL_EYE_OPEN, "Show");
    lv_list_add_button(list, LV_SYMBOL_EYE_CLOSE, "Hide");
    lv_list_add_button(list, LV_SYMBOL_TRASH, "Delete");
    lv_list_add_button(list, LV_SYMBOL_COPY, "Copy");
    lv_list_add_button(list, LV_SYMBOL_PASTE, "Paste");
}

static void text_input_create(lv_obj_t * parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * ta1 = lv_textarea_create(parent);
    lv_obj_set_width(ta1, LV_PCT(100));
    lv_textarea_set_one_line(ta1, true);
    lv_textarea_set_placeholder_text(ta1, "Click with an encoder to show a keyboard");

    lv_obj_t * ta2 = lv_textarea_create(parent);
    lv_obj_set_width(ta2, LV_PCT(100));
    lv_textarea_set_one_line(ta2, true);
    lv_textarea_set_placeholder_text(ta2, "Type something");

    lv_obj_t * kb = lv_keyboard_create(lv_screen_active());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_ALL, kb);
}

static void ta_event_cb(lv_event_t * e)
{
    lv_indev_t * indev = lv_indev_active();
    if(indev == NULL) return;
    lv_indev_type_t indev_type = lv_indev_get_type(indev);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED && indev_type == LV_INDEV_TYPE_ENCODER) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_group_focus_obj(kb);
        lv_group_set_editing(lv_obj_get_group(kb), kb != NULL);
        lv_obj_set_height(tv, LV_VER_RES / 2);
        lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    }

    if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(tv, LV_VER_RES);
    }
}