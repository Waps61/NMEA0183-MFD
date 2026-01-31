#ifndef MFD_CONF_H
#define MFD_CONF_H
#include <lvgl.h> // v9.3.0

// Configuration constants for MFD (Multi-Function Display)
#define MFD_SCREEN_WIDTH 1024
#define MFD_SCREEN_HEIGHT 600
#define BOX_BUFFER_SIZE 15
#define MFD_USE_DATABOX 1

#define JC1060P470
#define GFX_DEV_DEVICE JC1060P470
#define GFX_ROTATION 0
#define GFX_H_RES 1024
#define GFX_V_RES 600
#define GFX_BL 23 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define DSI_PANEL
#define TOUCH_SDA 7
#define TOUCH_SCL 8
#define TOUCH_INT 21
#define TOUCH_RST -1
#define DEFAULT_BRIGHTNESS 25 // 25% brightness as default
#define ACTUAL_BRIGHTNESS DEFAULT_BRIGHTNESS

static lv_subject_t brightness_subject;
static bool brightness_changed;
static int brightness_value;
static bool update_data_values;
static lv_obj_t *screen_active = NULL;

/*Create a Tabview box object*/
static lv_obj_t *SOGbox, *CTSbox, *COGbox, *DPTbox, *AWAbox, *TWAbox, *AWSbox, *TWSbox,
    *TRPbox, *VMGbox, *CMGbox, *LOGbox, *HDGbox = NULL;
// some boxes ure used twice
static lv_obj_t *SOGbox2, *COGbox2, *CTSbox2, *AWSbox2 = NULL;

static lv_obj_t *SOGvalue, *COGvalue, *CTSvalue, *DPTvalue, *AWAvalue, *TWAvalue,
    *AWSvalue, *TWSvalue, *TRPvalue, *VMGvalue, *CMGvalue, *LOGvalue, *Dummyvalue,
    *HDGvalue = NULL;
    //some values are used twice
static lv_obj_t *SOGvalue2, *COGvalue2, *CTSvalue2, *AWSvalue2 = NULL;

extern lv_obj_t *NMEAtabview,
    *NMEAtab_buttons, *NMEAtab1, *NMEAtab2, *NMEAtab3, *NMEAtab4, *NMEAtab5;

extern lv_obj_t *tab_array[];

/*A base style*/
extern lv_style_t style_base;

extern lv_style_t Databox_style, Databox_caption_style, Databox_unit_style;

static char Databoxbuffer[BOX_BUFFER_SIZE];

extern uint32_t NMEAtab_count;

#endif // MFD_CONF_H