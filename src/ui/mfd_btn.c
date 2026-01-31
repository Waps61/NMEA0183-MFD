/**
 * @file mfd_themes.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Button implementation to control the typcal color schemes for Day,Sun, Dawn and Night settings.
*/
/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_btn.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
  lv_style_t style_main;
  lv_style_t style_pressed;
} lv_button_styles_t;

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void mfd_button_style_observer_cb(lv_observer_t *observer, lv_subject_t *subject)
{
  LV_UNUSED(subject);

  lv_theme_mode_t m = (lv_theme_mode_t)lv_subject_get_int(&theme_subject);
  lv_button_styles_t *styles = (lv_button_styles_t *)lv_observer_get_target(observer);
  if (m == THEME_MODE_DAY)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAY_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(DAY_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAY_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(DAY_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_70);
  }
  if (m == THEME_MODE_NIGHT)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(NIGHT_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(NIGHT_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(NIGHT_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(NIGHT_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_30);
  }
  if (m == THEME_MODE_SUN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(SUN_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(SUN_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(SUN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(SUN_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_70);
  }
  if (m == THEME_MODE_DAWN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAWN_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(DAWN_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAWN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(DAWN_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_30);
  }

  lv_obj_report_style_change(&styles->style_main);
  lv_obj_report_style_change(&styles->style_pressed);
}

lv_obj_t *mfd_button_create(lv_obj_t *parent, const char *text)
{
  static lv_button_styles_t styles;
  if (!mfd_styles_inited)
    mfd_init_styles();
  lv_style_init(&styles.style_main);
  lv_style_init(&styles.style_pressed);
  styles.style_main = mfd_style_day;
  styles.style_pressed = mfd_style_dawn;

  lv_style_set_max_width(&styles.style_main,lv_pct(100));
  lv_style_set_max_width(&styles.style_pressed,lv_pct(100));
  lv_style_set_min_width(&styles.style_main, lv_pct(100));
  lv_style_set_min_width(&styles.style_pressed, lv_pct(100));
  //lv_style_set_max_height(&styles.style_main,100);
  //lv_style_set_max_height(&styles.style_pressed,100);
  lv_style_set_min_height(&styles.style_main, 75);
  lv_style_set_min_height(&styles.style_pressed, 75);
  lv_style_set_pad_all(&styles.style_main, 10);
  lv_style_set_pad_all(&styles.style_pressed, 10);
  lv_style_set_margin_all(&styles.style_main, 10);
  lv_style_set_margin_all(&styles.style_pressed, 10);
  lv_style_set_shadow_color(&styles.style_main, lv_color_hex(NIGHT_SURFACE));
  lv_style_set_shadow_color(&styles.style_pressed, lv_color_darken(lv_color_hex(NIGHT_SURFACE),LV_OPA_40));
  lv_style_set_shadow_width(&styles.style_main, 5);
  lv_style_set_shadow_width(&styles.style_pressed, 5);

  // {
  //   inited = true;

  //   lv_style_init(&styles.style_main);
  //   lv_style_set_radius(&styles.style_main, LV_RADIUS_CIRCLE);
  //   lv_style_set_bg_opa(&styles.style_main, LV_OPA_COVER);
  //   lv_style_set_bg_grad_dir(&styles.style_main, LV_GRAD_DIR_HOR);
  //   lv_style_set_shadow_width(&styles.style_main, 24);
  //   lv_style_set_shadow_offset_y(&styles.style_main, 6);
  //   lv_style_set_pad_hor(&styles.style_main, 32);
  //   lv_style_set_pad_ver(&styles.style_main, 12);

  //   lv_style_init(&styles.style_pressed);
  //   lv_style_set_color_filter_dsc(&styles.style_pressed, &lv_color_filter_shade);
  //   lv_subject_add_observer_with_target(&theme_subject, mfd_button_style_observer_cb, &styles, NULL);
  // }

  lv_obj_t *btn = lv_button_create(parent);
  lv_obj_set_name_static(btn, text);
  // lv_obj_remove_style_all(btn);
  lv_obj_add_style(btn, &styles.style_main, 0);
  lv_obj_add_style(btn, &styles.style_pressed, LV_STATE_PRESSED);
  //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, text);
  lv_obj_center(label);

  return btn;
}
