/**
 * Main Interface Configuration file
 */

#include "lvlconfig.h"

static struct repeating_timer lvgl_timer;
static lv_display_t *display;
static lv_draw_buf_t *buffer0;
static lv_draw_buf_t *buffer1;

bool repeating_lvgl_timer_callback(struct repeating_timer *t) {
    lv_tick_inc(10);
    return true;
}

static void dma_handler(void) {
    if (dma_channel_get_irq0_status(dma_tx)) {
        dma_channel_acknowledge_irq0(dma_tx);
        lv_disp_flush_ready(display);         /* Indicate you are ready with the flushing*/
    }
}

static void disp_flush_cb(lv_display_t *disp, const lv_area_t *area, unsigned char *color_p) {
    LCD_1IN28_SetWindows(area->x1, area->y1, area->x2 , area->y2);
    dma_channel_configure(dma_tx,
                          &channel_config,
                          &spi_get_hw(LCD_SPI_PORT)->dr,
                          color_p, // read address
                          ((area->x2 + 1 - area-> x1)*(area->y2 + 1 - area -> y1))*2,
                          true);
}


void LVGL_Init(void) {
    lv_init();

    display = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
    buffer0 = lv_draw_buf_create(DISP_HOR_RES, DISP_VER_RES, lv_display_get_color_format(display), 0);
    buffer1 = lv_draw_buf_create(DISP_HOR_RES, DISP_VER_RES, lv_display_get_color_format(display), 0);
    lv_display_set_buffers(display, buffer0, buffer1, DISP_HOR_RES * DISP_VER_RES / 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, disp_flush_cb);

    dma_channel_set_irq0_enabled(dma_tx, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    add_repeating_timer_ms(10,repeating_lvgl_timer_callback, NULL, &lvgl_timer);
}

void Widgets_Init(void) {
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label , "Hello World!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}
