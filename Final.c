#include "Types.h"
#include "LedMatrix.h"
#include "Events.h"
#include "Screen.h"


#include <stdio.h>
#include <hardware/timer.h>
#include <hardware/adc.h>

struct tm now;

Input_t input_buf[INPUT_BUFF_SIZE] = {IP_NONE};


bool repeating_timer_LM_blink(struct repeating_timer *t) {
    LM_update();
    return true;
}

bool repeating_timer_LM_nextHour(struct repeating_timer *t) {
    LM_nextBlinkLed();
    return true;
}

void newEvent(Event *event) {
    Ev_newEvent(event);
    if (Tp_timeSameDay(&(event->begin), &now)) {
        int h;
        for (h=1; h<=24; ++h) {
            if (h >= event->begin.tm_hour && h <= event->end.tm_hour) {
                LM_setHourColor(h, event->color.r, event->color.g, event->color.b);
            }
        }
    }
}

void addInput(Input_t input) {
    for (int8_t i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_NONE) {
            input_buf[i] = input;
            return;
        }
    }
}

void gpio_irq_Buttom(uint gpio, uint32_t events) {
    Input_t input;
    switch (gpio) {
    case A_BUTTOM_PIN:
        input = IP_BUTTOM_A;
        break;
    case B_BUTTOM_PIN:
        input = IP_BUTTOM_B;
        break;
    case ANALOGIC_BUTTOM_PIN:
        input = IP_BUTTOM_ANALOG;
        break;
    default:
        return;
    }
    addInput(input);
}

void clearInput() {
    for (int8_t i=0; i<INPUT_BUFF_SIZE; ++i) {
        input_buf[i] = IP_NONE;
    }
}

void getAnalog() {
    uint16_t read;
    adc_select_input(ADC_GPIO_INPUT(ANALOGIC_X_PIN));
    read = adc_read();
    if (read > 0xC00) {
        addInput(IP_ANALOG_RIGHT);
    } else if (read < 0x0040) {
        addInput(IP_ANALOG_LEFT);
    }
    adc_select_input(ADC_GPIO_INPUT(ANALOGIC_Y_PIN));
    read = adc_read();
    if (read > 0xC00) {
        addInput(IP_ANALOG_UP);
    } else if (read < 0x0040) {
        addInput(IP_ANALOG_DOWN);
    }
}

void setupInput() {
    clearInput();

    gpio_init(A_BUTTOM_PIN);
    gpio_set_dir(A_BUTTOM_PIN, GPIO_IN);
    gpio_pull_up(A_BUTTOM_PIN);
    gpio_set_irq_enabled_with_callback(A_BUTTOM_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_Buttom);

    gpio_init(B_BUTTOM_PIN);
    gpio_set_dir(B_BUTTOM_PIN, GPIO_IN);
    gpio_pull_up(B_BUTTOM_PIN);
    gpio_set_irq_enabled_with_callback(B_BUTTOM_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_Buttom);

    gpio_init(ANALOGIC_BUTTOM_PIN);
    gpio_set_dir(ANALOGIC_BUTTOM_PIN, GPIO_IN);
    gpio_pull_up(ANALOGIC_BUTTOM_PIN);
    gpio_set_irq_enabled_with_callback(ANALOGIC_BUTTOM_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_Buttom);

    adc_init();
    adc_gpio_init(ANALOGIC_X_PIN);
    adc_gpio_init(ANALOGIC_Y_PIN);
}

int main() {
    stdio_init_all();
    LM_setup();
    aon_timer_start_with_timeofday();

    aon_timer_get_time_calendar(&now);

    Event event = {
        .name = "Oi",
        .alert = true,
        .color = {0, 255, 0},
        .begin = {.tm_hour = 21, .tm_mday=now.tm_mday, .tm_mon=now.tm_mon, .tm_year=now.tm_year},
        .end = {.tm_hour = 22, .tm_mday=now.tm_mday, .tm_mon=now.tm_mon, .tm_year=now.tm_year}
    };
    
    newEvent(&event);

    struct repeating_timer LM_blink_timer;
    add_repeating_timer_ms(LM_BLINK_PERIOD, repeating_timer_LM_blink, NULL,  &LM_blink_timer);
    
    struct repeating_timer LM_next_hour_timer;
    add_repeating_timer_ms(LM_NEXT_HOUR_PERIOD, repeating_timer_LM_nextHour, NULL, &LM_next_hour_timer);

    Dp_setup();

    setupInput();

   Screen screen;
   Sc_changeScreen(&screen, SC_MENU);
    
    while (true) {
        getAnalog();
        screen.update(&event, input_buf);
        clearInput();
        sleep_ms(1000);
    }
}