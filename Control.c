#include "Control.h"

#include "Screen.h"
#include "Display.h"
#include "LedMatrix.h"
#include "Events.h"

#include <stdio.h>
#include <pico/aon_timer.h>
#include <hardware/adc.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>

Input_t input_buf[INPUT_BUFF_SIZE] = {IP_NONE};
struct tm now;
Event event = {
    .alert = false,
    .color = {255,255,255},
    .name = " "
};

Event dayEvents[MAX_DAY_EVENTS];
uint8_t numDayEvents = 0;

struct repeating_timer checkAnalog_timer;
struct repeating_timer minute_check_timer;


inline uint8_t stoi(char c) {
    return (c>='0' && c<='9')? c-'0' : 0;
}

void Ctr_setup_aon_timer() {
    char date[] = __DATE__;
    static struct tm init_tm;

    init_tm.tm_year = stoi(date[7])*1000 + stoi(date[8])*100 + stoi(date[9])*10 + stoi(date[10]);
    init_tm.tm_mday = stoi(date[4])*10 + stoi(date[5]);
    
    if (date[0] == 'J' && date[1] == 'a' && date[2] == 'n') init_tm.tm_mon = 1;
    else if (date[0] == 'F' && date[1] == 'e' && date[2] == 'b') init_tm.tm_mon = 2;
    else if (date[0] == 'M' && date[1] == 'a' && date[2] == 'r') init_tm.tm_mon = 3;
    else if (date[0] == 'A' && date[1] == 'p' && date[2] == 'r') init_tm.tm_mon = 4;
    else if (date[0] == 'M' && date[1] == 'a' && date[2] == 'y') init_tm.tm_mon = 5;
    else if (date[0] == 'J' && date[1] == 'u' && date[2] == 'n') init_tm.tm_mon = 6;
    else if (date[0] == 'J' && date[1] == 'u' && date[2] == 'l') init_tm.tm_mon = 7;
    else if (date[0] == 'A' && date[1] == 'u' && date[2] == 'g') init_tm.tm_mon = 8;
    else if (date[0] == 'S' && date[1] == 'e' && date[2] == 'p') init_tm.tm_mon = 9;
    else if (date[0] == 'O' && date[1] == 'c' && date[2] == 't') init_tm.tm_mon = 10;
    else if (date[0] == 'N' && date[1] == 'o' && date[2] == 'v') init_tm.tm_mon = 11;
    else if (date[0] == 'D' && date[1] == 'e' && date[2] == 'c') init_tm.tm_mon = 12;
    else init_tm.tm_mon = 0;

    char time[] = __TIME__;
    init_tm.tm_hour = stoi(time[0])*10 + stoi(time[1]);
    init_tm.tm_min = stoi(time[3])*10 + stoi(time[4]);
    init_tm.tm_sec = stoi(time[6])*10 + stoi(time[7]);

    
    
    aon_timer_start_calendar(&init_tm);
    sleep_ms(100);
    aon_timer_get_time_calendar(&now);
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

bool repeating_timer_checkAnalog(struct repeating_timer *t) {
    getAnalog();
    return true;
}

void Ctr_setupInput() {
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
    add_repeating_timer_ms(INPUT_CHECK_ANALOG_PERIOD, repeating_timer_checkAnalog, NULL, &checkAnalog_timer);

}

void Ctr_setupBuzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys)/(BUZZER_FREQ*4096));
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(BUZZER_PIN, 0);
}

int64_t alarm_stopBuzzerBeep(alarm_id_t id, __unused void *user_data) {
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

void Ctr_buzzerBeep(uint32_t time) {
    pwm_set_gpio_level(BUZZER_PIN, BUZZER_DUTTY);
    add_alarm_in_ms(time, alarm_stopBuzzerBeep, NULL, false);
}

void Ctr_setLMEventColor(struct tm *begin, struct tm *end, Color color) {
    for (int h=1; h<=24; ++h) {
        if (h >= begin->tm_hour && h <= end->tm_hour) {
            LM_setHourColor(h, color.r, color.g, color.b);
        }
    }
}

void Ctr_addDayEvent(Event *ev) {
    if (numDayEvents < MAX_DAY_EVENTS) {
        Ctr_setLMEventColor(&(ev->begin), &(ev->end), ev->color);
        dayEvents[numDayEvents++] = *ev;
    }
}

void Ctr_addEvent(Event *ev) {
    Ev_newEvent(ev);
    if (Tp_timeSameDay(&(ev->begin), &now)) {
        Ctr_addDayEvent(ev);
    }
}

void Ctrl_nextDay() {
    numDayEvents = 0;

    Event ev;
    int i = 0;
    while (Ev_getEvent(&ev, i++)) {
        if (Tp_timeSameDay(&(ev.begin), &now)) {
            Ctr_addDayEvent(&ev);
        }
    }
}

bool repeating_timer_minuteCheck(struct repeating_timer *t) {
    LM_setBlinkLed((now.tm_hour) ? now.tm_hour : 24);

    if (now.tm_hour == 0 && now.tm_min == 0) {
        Ctrl_nextDay();
    }

    for (int i=0; i<numDayEvents; i++) {
        if (Tp_timeSameMin(&(dayEvents[i].begin), &now) && dayEvents[i].alert) {
            Ctr_buzzerBeep(BUZZER_BEEP_TIME);
        }
    }
}

void Ctr_setupAll() {
    Ctr_setup_aon_timer();
    Dp_setup();
    LM_setup();
    LM_setBlinkLed(now.tm_hour);
    Ctr_setupInput();
    Ctr_setupBuzzer();
    
    add_repeating_timer_ms(60*1000, repeating_timer_minuteCheck, NULL, &minute_check_timer);
}

void Ctr_menuLoop() {
    static Screen screen;
    static Sc_Type screen_type = SC_MENU;
    static bool first = true;
    aon_timer_get_time_calendar(&now);
    if (first) {
        Sc_changeScreen(&screen, screen_type);
        first = false;
        event.begin = now;
        event.end = now;
    }

    Sc_Type new_screen_type = screen.update(&event, input_buf);
    if (new_screen_type != screen_type) {
        if (new_screen_type == SC_NEW_EVENT_F) {
            Ctr_addEvent(&event);
            new_screen_type = SC_MENU;
        }
        screen_type = new_screen_type;
        event.begin = now;
        event.end = now;
        Sc_changeScreen(&screen, screen_type);
    }
    if (screen_type == SC_MENU) {
        event.begin = now;
    }
    clearInput();

}