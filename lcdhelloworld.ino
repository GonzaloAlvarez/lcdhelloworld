#include <stdio.h>
#include "lvlconfig.h"

void setup() {
    DEV_Module_Init();
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);
    LVGL_Init();
    Widgets_Init();
}

void loop() {
    lv_timer_handler();
    DEV_Delay_ms(10);
}