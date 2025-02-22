#include "Types.h"
#include "Control.h"

#include <stdio.h>
#include <hardware/timer.h>
#include <hardware/adc.h>

int main() {
    stdio_init_all();
    Ctr_setupAll();
    
    while (true) {
        Ctr_menuLoop();
        sleep_ms(250);
    }
}