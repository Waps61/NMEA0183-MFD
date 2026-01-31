/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Config (settings) screen implementation. The settings screen is used to modify program settings
  like baudrate, WiFi switch, adres* and password* (* to be done) and the total mileage log
*/ 
#include <ui/mfd_config_panel.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>

// create the config screen
void mfd_config_panel_create(lv_obj_t *parent)
{
  lv_log("creating panel %s\n",lv_obj_get_name(parent));
  {
    lv_obj_t *baudlabel = lv_label_create(parent);
    lv_obj_set_pos(baudlabel, 30, 36);
    lv_obj_set_size(baudlabel, 250, 25);
    lv_obj_set_style_text_font(baudlabel, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(baudlabel, lv_color_hex(0xff515050), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(baudlabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(baudlabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(baudlabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(baudlabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(baudlabel, "Baudrate");
  }
  {
    lv_obj_t *baudvalue = lv_dropdown_create(parent);
    lv_obj_set_pos(baudvalue, 300, 29);
    lv_obj_set_size(baudvalue, 200, 43);
    lv_dropdown_set_options(baudvalue, "4800\n38400");
    lv_dropdown_set_selected(baudvalue, 1);
    lv_obj_set_style_text_font(baudvalue, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  {
    lv_obj_t *wifilabel = lv_label_create(parent);
    lv_obj_set_pos(wifilabel, 100, 103);
    lv_obj_set_size(wifilabel, 130, 25);
    lv_obj_set_style_text_font(wifilabel, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(wifilabel, lv_color_hex(0xff515050), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(wifilabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(wifilabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(wifilabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(wifilabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(wifilabel, "WiFi");
  }
  {
    lv_obj_t *wifivalue = lv_switch_create(parent);
    lv_obj_set_pos(wifivalue, 300, 94);
    lv_obj_set_size(wifivalue, 50, 34);
  }
  {
    lv_obj_t *loglabel = lv_label_create(parent);
    lv_obj_set_pos(loglabel, 0, 188);
    lv_obj_set_size(loglabel, 306, 35);
    lv_obj_set_style_text_font(loglabel, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(loglabel, lv_color_hex(0xff515050), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(loglabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(loglabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(loglabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(loglabel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(loglabel, "Log-ofsset");
  }

  {
    // logTextbox
    lv_obj_t *logvalue = lv_textarea_create(parent);
    lv_obj_set_pos(logvalue, 300, 179);
    lv_obj_set_size(logvalue, 150, 70);
    lv_textarea_set_accepted_chars(logvalue, "1234567890");
    lv_textarea_set_max_length(logvalue, 6);

    lv_obj_add_flag(logvalue, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(logvalue, LV_STATE_FOCUSED | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(logvalue, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(logvalue, lv_color_hex(0xffcc229c), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(logvalue, 255, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_textarea_set_placeholder_text(logvalue, "log");
    lv_textarea_set_one_line(logvalue, false);
    lv_textarea_set_password_mode(logvalue, false);
  }
  {
    // cfgKeyboard
    lv_obj_t *kbd = lv_keyboard_create(parent);
    lv_obj_set_pos(kbd, 0, 300);
    lv_obj_set_size(kbd, 795, 216);
    lv_keyboard_set_mode(kbd, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_set_style_align(kbd, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(kbd, &lv_font_montserrat_26, LV_PART_ITEMS | LV_STATE_DISABLED);
  }
  {
    // SaveBtn
    lv_obj_t *savebtn = lv_button_create(parent);
    lv_obj_set_pos(savebtn, 625, 108);
    lv_obj_set_size(savebtn, 175, 146);
    lv_obj_set_style_text_font(savebtn, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(savebtn, lv_color_hex(0xff2bc33b), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(savebtn, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(savebtn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    {

      lv_obj_t *btnlbl = lv_label_create(savebtn);
      lv_obj_set_pos(btnlbl, 0, 0);
      lv_obj_set_size(btnlbl, 127, 45);
      lv_obj_set_style_align(btnlbl, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(btnlbl, &ui_font_lv_conthrax_24, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_label_set_text(btnlbl, "Save");
      lv_obj_set_style_text_align(btnlbl, LV_TEXT_ALIGN_CENTER,0);
    }
  }
}
