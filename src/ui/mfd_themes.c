/**
 * @file mfd_themes.c
 */

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 * STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*
 * eventhandler for switching themes
 */
static void switch_theme_event_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  switch (lv_subject_get_int(&theme_subject))
  {
  case THEME_MODE_NIGHT:
    lv_subject_set_int(&theme_subject, THEME_MODE_NIGHT);
    break;

  case THEME_MODE_DAWN:
    lv_subject_set_int(&theme_subject, THEME_MODE_DAWN);
    break;

  case THEME_MODE_SUN:
    lv_subject_set_int(&theme_subject, THEME_MODE_SUN);
    break;

  default:
    lv_subject_set_int(&theme_subject, THEME_MODE_DAY);
    break;
  }
}

void mfd_init_styles()
{
  mfd_styles_inited = true;
  lv_style_init(&mfd_style_day);
  lv_style_set_bg_color(&mfd_style_day, lv_color_hex(DAY_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_day, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_day, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_text_opa(&mfd_style_day, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_day, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_day, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_day, 1);
  lv_style_set_text_font(&mfd_style_night, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_day, LV_ALIGN_CENTER);

  lv_style_init(&mfd_style_night);
  lv_style_set_bg_color(&mfd_style_night, lv_color_hex(NIGHT_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_night, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_night, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_night, LV_OPA_50);
  lv_style_set_border_color(&mfd_style_night, lv_color_hex(NIGHT_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_night, 1);
  lv_style_set_text_font(&mfd_style_night, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_night, LV_ALIGN_CENTER);

  lv_style_init(&mfd_style_sun);
  lv_style_set_bg_color(&mfd_style_sun, lv_color_hex(SUN_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_sun, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_sun, lv_color_hex(SUN_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_sun, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_sun, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_sun, lv_color_hex(SUN_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_sun, 1);
  lv_style_set_text_font(&mfd_style_sun, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_sun, LV_ALIGN_CENTER);

  lv_style_init(&mfd_style_dawn);
  lv_style_set_bg_color(&mfd_style_dawn, lv_color_hex(DAWN_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_dawn, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_dawn, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_dawn, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_dawn, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_dawn, lv_color_hex(DAWN_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_dawn, 1);
  lv_style_set_text_font(&mfd_style_dawn, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_dawn, LV_ALIGN_CENTER);

  mfd_style = mfd_style_day;
}