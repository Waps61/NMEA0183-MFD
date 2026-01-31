#ifndef MFD_CONFIG_PANEL_H
#define MFD_CONFIG_PANEL_H

#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>


#ifdef __cplusplus
extern "C"
{
#endif

    void mfd_config_panel_create(lv_obj_t *parent);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_BRIGHT_PANEL_H