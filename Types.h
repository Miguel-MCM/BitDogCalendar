#ifndef TYPES_H
#define TYPES_H

#include "pico/stdlib.h"
#include <pico/aon_timer.h>

#define LED_COUNT 25
#define LED_PIN 7

#define LM_BLINK_PERIOD 1000
#define LM_NEXT_HOUR_PERIOD (10*1000)

#define MAX_EVENT_NAME_LEN 50

#define MAX_EVENT_NUM 100

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define DISPLAY_I2C_PORT i2c1
#define DISPLAY_I2C_SDA 14
#define DISPLAY_I2C_SCL 15
#define DISPLAY_I2C_FREQ (400*1000)
#define DISPLAY_I2C_ADDR _u(0x3C)

#define DISPLAY_CHAR_SIZE 8

#define MAX_SCREEN_STR_LEN 150

#define A_BUTTOM_PIN 5
#define B_BUTTOM_PIN 6
#define ANALOGIC_BUTTOM_PIN 22
#define ANALOGIC_X_PIN 27
#define ANALOGIC_Y_PIN 26
#define ADC_GPIO_INPUT(pin) ((pin)-26)

#define INPUT_BUFF_SIZE 16
#define INPUT_CHECK_ANALOG_PERIOD 425

typedef struct Color {
    uint8_t r, g, b;
} Color;

typedef uint8_t EventIndex_t;

typedef struct Event {
    char name[MAX_EVENT_NAME_LEN];
    struct tm begin;
    struct tm end;
    Color color;
    bool alert;
    EventIndex_t index;
} Event;

bool Tp_timeIn(const struct tm *tm1,const struct tm *begin, const struct tm *end);
bool Tp_timeSameDay(const struct tm *tm1, const struct tm *tm2);

typedef enum Input_t {
    IP_NONE,
    IP_BUTTOM_A,
    IP_BUTTOM_B,
    IP_BUTTOM_ANALOG,
    IP_ANALOG_UP,
    IP_ANALOG_DOWN,
    IP_ANALOG_LEFT,
    IP_ANALOG_RIGHT
} Input_t;

typedef enum Sc_Type {
    SC_MENU,
    SC_NEW_EVENT,
    SC_NEW_EVENT_F
} Sc_Type;

typedef struct Screen {
    char text[MAX_SCREEN_STR_LEN];
    Sc_Type type;
    Sc_Type (*update)(Event*, Input_t*);
} Screen;


#endif