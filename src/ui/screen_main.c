/**
 * @file screen_main_gen.c
 * @brief Template source file for LVGL objects
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Main screen implementation. The main screen is a static screen. On top al other panels are stacked.
  The main screen initialises and controls the menubar and the 5 panels
  Trip, Wind and Course panels are used to display max 6 tiles per panel
  Brightness and Settings apnel have their own helper functions defined
*/
/*********************
 *      INCLUDES
 *********************/
#include <Arduino.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mfd_conf.h>
#include <ui/screen_main.h>
#include <ui/screen_about.h>
#include <ui/ui_screens.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_panel.h>
#include <ui/mfd_btn.h>
#include <ui/mfd_tile.h>
#include <ui/mfd_tile_data.h>
#include <NMEA0183_data.h>
#include <ui/mfd_bright_panel.h>
#include <ui/mfd_config_panel.h>
#include <esp_heap_caps.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *mfd_menubar = NULL;          // a static menu bar that is used to selected a mfd panel
static lv_obj_t *mfd_trip_panel = NULL;       // a static panel to display trip data
static lv_obj_t *mfd_wind_panel = NULL;       // a static panel to display wind data
static lv_obj_t *mfd_course_panel = NULL;     // a static panel to display course data
static lv_obj_t *mfd_brightness_panel = NULL; // a static panel for brigtness settings
static lv_obj_t *mfd_config_panel = NULL;     // a static panel for the config setting
static char tile_data_buffer[15];

unsigned long previous_millis = 0;
unsigned long current_millis = 0;
int mileage = 0;

size_t free_bytes;
/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void test_screen_data_updates()
{
  current_millis = millis();
  // if (millis() % 3600)
  // {
  sprintf(tile_data_buffer, "%d", mileage);
  lv_label_set_text(TRPvalue, tile_data_buffer);
  mileage = (mileage++) % 1001;
  // }
  if (millis() - previous_millis > UPDATE_DELAY)
  {
    previous_millis = current_millis;
    boat_cts = (int)(rand() % 10) + 270;
    sprintf(tile_data_buffer, "%d", boat_cts);
    lv_label_set_text(CTSvalue, tile_data_buffer);
    lv_label_set_text(CTSvalue2, tile_data_buffer);
    boat_hdg = (int)(rand() % 10) + 265;
    sprintf(tile_data_buffer, "%d", boat_hdg);
    lv_label_set_text(HDGvalue, tile_data_buffer);
    boat_cog = (int)(rand() % 5) + 270;
    sprintf(tile_data_buffer, "%d", boat_cog);
    lv_label_set_text(COGvalue, tile_data_buffer);
    lv_label_set_text(COGvalue2, tile_data_buffer);
    boat_sog = (float)(rand() % 20) / 10 + 6;
    sprintf(tile_data_buffer, "%.1f", boat_sog);
    lv_label_set_text(SOGvalue, tile_data_buffer);
    lv_label_set_text(SOGvalue2, tile_data_buffer);

    sprintf(tile_data_buffer, "%.1f", (float)(rand() % 25) / 10 + 5);
    lv_label_set_text(DPTvalue, tile_data_buffer);
    sprintf(tile_data_buffer, "%d", (int)(rand() % 4) + 13);
    lv_label_set_text(AWSvalue, tile_data_buffer);
    lv_label_set_text(TWSvalue, tile_data_buffer);
    lv_label_set_text(AWSvalue2, tile_data_buffer);
    boat_awa = (int)(rand() % 5) + 30;
    sprintf(tile_data_buffer, "%d", boat_awa);
    lv_label_set_text(AWAvalue, tile_data_buffer);

    sprintf(tile_data_buffer, "%.1f", (float)(boat_sog * cos(boat_awa * PI / 180)));
    lv_label_set_text(VMGvalue, tile_data_buffer);

    sprintf(tile_data_buffer, "%d", (int)(rand() % 5) + 40);
    lv_label_set_text(TWAvalue, tile_data_buffer);
    sprintf(tile_data_buffer, "%d", mileage++);
    lv_label_set_text(TRPvalue, tile_data_buffer);
    sprintf(tile_data_buffer, "%.1f", (float)(rand() % 25) / 10 + 5);

    sprintf(tile_data_buffer, "%d", boat_cog-boat_cts);
    lv_label_set_text(CMGvalue, tile_data_buffer);
  }
}

void menu_btn_event_cb(lv_event_t *event)
{
  lv_obj_t *panel_btn = NULL;
  panel_btn = (lv_obj_t *)lv_event_get_user_data(event);
  // char *panel_name = lv_obj_get_name(panel_btn);
  // lv_log("panel to show is %s\n", panel_name);
  // lv_log(" panels equal is: %d\n", strcmp(panel_name, "TRIP"));
  // if (strcmp(panel_name, "TRIP") == 0)
  // {
  //   lv_log("hidding panel: %s\n)", panel_name);
  //   if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
  //     mfd_show_panel(panel_btn);
  //   lv_log("hidding panel: WIND\n");
  //   mfd_hide_panel(mfd_wind_panel);
  //   lv_log("hidding panel: COURSE\n");
  //   mfd_hide_panel(mfd_course_panel);
  //   lv_log("hidding panel: BRIGHTNESS\n");
  //   mfd_hide_panel(mfd_brightness_panel);
  //   lv_log("hidding panel: SETTING\n");
  //   mfd_hide_panel(mfd_config_panel);
  // }
  // else if (strcmp(panel_name, "WIND") == 0)
  // {
  //   if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
  //     mfd_show_panel(panel_btn);
  //   mfd_hide_panel(mfd_trip_panel);
  //   mfd_hide_panel(mfd_course_panel);
  //   mfd_hide_panel(mfd_brightness_panel);
  //   mfd_hide_panel(mfd_config_panel);
  // }
  if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
    mfd_show_panel(panel_btn);
  else
    mfd_hide_panel(panel_btn);
}

lv_obj_t *screen_main_create(void)
{
  LV_TRACE_OBJ_CREATE("begin");

  static bool style_inited = false;

  if (!style_inited)
  {
    mfd_init_styles();
    style_inited = true;
  }

  if (screen_main == NULL)
    screen_main = lv_obj_create(NULL);
  // Create the main screen as a permanent screen
  //lv_obj_t *lv_obj_0 = screen_main;
  //screen_active = lv_obj_create(screen_main);
  screen_active = screen_main;
  //lv_log("screen active = %s", lv_obj_get_name(screen_main));
  lv_obj_set_name_static(screen_active, "screen_main_#");

  // Add a menubar
  lv_obj_t *menu_bar = lv_obj_create(screen_active);
  lv_obj_remove_style_all(menu_bar);
  lv_obj_set_flex_flow(menu_bar, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_style(menu_bar, &mfd_style_dawn, LV_STATE_DEFAULT);
  lv_obj_set_x(menu_bar, 0);
  lv_obj_set_y(menu_bar, 0);
  lv_obj_set_style_radius(menu_bar, 5, 0);
  lv_obj_set_style_margin_all(menu_bar, 5, 0);
  lv_obj_set_style_bg_color(menu_bar, lv_palette_main(LV_PALETTE_DEEP_ORANGE), LV_STATE_DEFAULT);

  lv_obj_set_size(menu_bar, 150, lv_pct(100));
  lv_obj_set_style_bg_color(menu_bar, lv_color_hex(NIGHT_BACKGROUND), 0);

  // Create the panels TRIP, WIND, COURSE, BRIGHTNESS and SETTINGS
  lv_obj_t *mfd_trip_panel = mfd_panel_create(screen_active, "TRIP");
  //lv_obj_add_style(mfd_trip_panel, mfd_style, 0);
  lv_obj_t *mfd_wind_panel = mfd_panel_create(screen_active, "WIND");
  mfd_hide_panel(mfd_wind_panel);
  lv_obj_t *mfd_course_panel = mfd_panel_create(screen_active, "COURSE");
  mfd_hide_panel(mfd_course_panel);
  lv_obj_t *mfd_bright_panel = mfd_panel_create(screen_active, "BRIGHTNESS");
  mfd_hide_panel(mfd_bright_panel);
  lv_obj_t *mfd_settings_panel = mfd_panel_create(screen_active, " SETTING");
  mfd_hide_panel(mfd_settings_panel);

  // Add the buttons and their link to their panel to the menubar
  // The void *userdata reference  is the reference to the panel to show
  lv_obj_t *trip_btn = mfd_button_create(menu_bar, "TRIP");
  lv_obj_add_flag(trip_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(trip_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_trip_panel);

  lv_obj_t *wind_btn = mfd_button_create(menu_bar, "WIND");
  lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(wind_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_wind_panel);

  lv_obj_t *course_btn = mfd_button_create(menu_bar, "COURSE");
  lv_obj_add_event_cb(course_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_course_panel);

  lv_obj_t *bright_btn = mfd_button_create(menu_bar, "BRIGHT");
  lv_obj_add_event_cb(bright_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_bright_panel);

  lv_obj_t *setting_btn = mfd_button_create(menu_bar, "SETTING");
  lv_obj_add_event_cb(setting_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_settings_panel);

  lv_obj_t *lv_button_0 = mfd_button_create(menu_bar, "(i)");

  // Create the about screen as a child of the main screen
  lv_obj_add_screen_create_event(lv_button_0, LV_EVENT_CLICKED, screen_about_create, LV_SCREEN_LOAD_ANIM_MOVE_TOP, 500, 0);

  // Add the tiles and their tile_data objects to the trip panel
  CTSbox = mfd_panel_add_tile(mfd_trip_panel, "CTS", "o", CTSbox);
  CTSvalue = mfd_tile_add_tile_data(CTSbox, CTSvalue);
  // lv_obj_add_event_cb(CTSvalue, mfd_tile_data_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  COGbox = mfd_panel_add_tile(mfd_trip_panel, "COG", "o", COGbox);
  COGvalue = mfd_tile_add_tile_data(COGbox, COGvalue);

  SOGbox = mfd_panel_add_tile(mfd_trip_panel, "SOG", "KTS", SOGbox);
  SOGvalue = mfd_tile_add_tile_data(SOGbox, SOGvalue);

  DPTbox = mfd_panel_add_tile(mfd_trip_panel, "DPT", "m", DPTbox);
  DPTvalue = mfd_tile_add_tile_data(DPTbox, DPTvalue);

  AWSbox = mfd_panel_add_tile(mfd_trip_panel, "AWS", "KTS", AWSbox);
  AWSvalue = mfd_tile_add_tile_data(AWSbox, AWSvalue);

  TRPbox = mfd_panel_add_tile(mfd_trip_panel, "TRP", "nm", TRPbox);
  TRPvalue = mfd_tile_add_tile_data(TRPbox, TRPvalue);

  // Add the tiles and their tile_data objects to thwe wind panel
  AWAbox = mfd_panel_add_tile(mfd_wind_panel, "AWA", "o", AWAbox);
  AWAvalue = mfd_tile_add_tile_data(AWAbox, AWAvalue);
  // lv_obj_add_event_cb(CTSvalue, mfd_tile_data_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  TWAbox = mfd_panel_add_tile(mfd_wind_panel, "TWA", "o", TWAbox);
  TWAvalue = mfd_tile_add_tile_data(TWAbox, TWAvalue);

  AWSbox2 = mfd_panel_add_tile(mfd_wind_panel, "AWS", "KTS", AWSbox2);
  AWSvalue2 = mfd_tile_add_tile_data(AWSbox2, AWSvalue2);

  TWSbox = mfd_panel_add_tile(mfd_wind_panel, "TWS", "KTS", SOGbox);
  TWSvalue = mfd_tile_add_tile_data(TWSbox, TWSvalue);

  // Add the tiles and their tile_data objects to thwe course panel
  CTSbox2 = mfd_panel_add_tile(mfd_course_panel, "CTS", "o", CTSbox2);
  CTSvalue2 = mfd_tile_add_tile_data(CTSbox2, CTSvalue2);

  HDGbox = mfd_panel_add_tile(mfd_course_panel, "HDG", "o", HDGbox);
  HDGvalue = mfd_tile_add_tile_data(HDGbox, HDGvalue);

  COGbox2 = mfd_panel_add_tile(mfd_course_panel, "COG", "o", COGbox2);
  COGvalue2 = mfd_tile_add_tile_data(COGbox2, COGvalue2);

  SOGbox2 = mfd_panel_add_tile(mfd_course_panel, "SOG", "KTS", SOGbox2);
  SOGvalue2 = mfd_tile_add_tile_data(SOGbox2, SOGvalue2);

  VMGbox = mfd_panel_add_tile(mfd_course_panel, "VMG", "KTS", VMGbox);
  VMGvalue = mfd_tile_add_tile_data(VMGbox, VMGvalue);

  CMGbox = mfd_panel_add_tile(mfd_course_panel, "CMG", "0", CMGbox);
  CMGvalue = mfd_tile_add_tile_data(CMGbox, CMGvalue);

  mfd_brightness_panel_create(mfd_bright_panel);
  mfd_config_panel_create(mfd_settings_panel);

  LV_TRACE_OBJ_CREATE("finished");

  return screen_active;
}
