#include "Display.h"

#include "ssd1306.h"
#include "Types.h"

#include <stdio.h>
#include <string.h>

uint8_t ssd[ssd1306_buffer_length];
struct render_area frame_area;

void Dp_setup() {
    i2c_init(DISPLAY_I2C_PORT, DISPLAY_I2C_FREQ);

    gpio_set_function(DISPLAY_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_I2C_SDA);
    gpio_pull_up(DISPLAY_I2C_SCL);

    ssd1306_init();

    frame_area.start_column = 0;
    frame_area.end_column = DISPLAY_WIDTH - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;

    calculate_render_area_buffer_length(&frame_area);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

}

void Dp_drawString(char *str, int16_t x, int16_t y) {
    ssd1306_draw_string(ssd, x, y, str);
}

void Dp_update() {
    render_on_display(ssd, &frame_area);
}