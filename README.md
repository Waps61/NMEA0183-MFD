# NMEA0183-MFD
An attempt to get NMEA1083 data displayed on a MFD JC1060P470 with an ESp32-P4-C6 on board with LVGL
Because I could not get the provided examples to run. Most examples have faulty references to libraries
Nor does it support the graphics driver in the GFX library for Arduino.
Second LVGL is a new library, which has a steep learning curve with partly documentation
In the lvgl.h files you also need to ativate several options to make use of specific functionality 

If you (like me) assume that, with all your experience, this is a walk in the park, then this is bad news.
1) You need to RTFM to get started and to get your installation right for LVGL
2) Start small with a simple exercise to get an understanding of the working
3) make a plan

So the 1st thing to do is to create the lvgl-conf.h (make a copy of lv_conf_template.h  in the lvfgl folder),
and rename to lvgl-conf.h 1 level up in the tree, so it risides in the same directory as the lvgl folder.
2nd open the lvgl-conf.h file and at line 15 (as instructed in the file) below\
>`/* clang-format off */\
#if 0 /* Set this to "1" to enable content */`\
set this to 1.

There many more of these settings that are set to off by default!!
Next you have to make create the setting for the displaydriver jd9165 like below:

Internet research got me this. BIG Thanks for whoever made this
>`Arduino_ESP32DSIPanel *dsipanel = new Arduino_ESP32DSIPanel(
    40 /* hsync_pulse_width */, 160 /* hsync_back_porch */, 160 /* hsync_front_porch */,
    10 /* vsync_pulse_width */, 23 /*vsync_back_porch  */, 12 /* vsync_front_porch */,
    48000000 /* prefer_speed */);`\
>`Arduino_DSI_Display *gfx = new Arduino_DSI_Display(
    GFX_H_RES /* width */, GFX_V_RES /* height */, dsipanel, GFX_ROTATION /* rotation */, true /* auto_flush */,
    27 /* RST */, jd9165_init_operations, sizeof(jd9165_init_operations) / sizeof(lcd_init_cmd_t));`\
See the NMEA0183_conf.h file for more details to get this display up and running\
## Example
![screenshot of the mainscreen](/Waps61/NMEA0183-MFD/blob/main/screenshots/IMG_4331.jpg))
