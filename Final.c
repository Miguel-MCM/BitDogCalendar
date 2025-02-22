#include "Types.h"
#include "Control.h"
#include "Screen.h"
#include "Display.h"
#include "LedMatrix.h"

#include <stdio.h>
#include <hardware/timer.h>
#include <hardware/adc.h>

bool repeating_timer_LM_nextHour(struct repeating_timer *t) {
    LM_nextBlinkLed();
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(1000);
    Ctr_setupAll();
    struct repeating_timer LM_next_hour_timer;
    add_repeating_timer_ms(LM_NEXT_HOUR_PERIOD, repeating_timer_LM_nextHour, NULL, &LM_next_hour_timer);
    
    while (true) {
        Ctr_menuLoop();
        sleep_ms(250);
    }
}