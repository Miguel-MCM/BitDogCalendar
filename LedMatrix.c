#include "LedMatrix.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "ws2818b.pio.h"

volatile Color leds[LED_COUNT];
volatile uint blinking_hour = 0;
volatile bool blinking_led_on = true;
volatile Color blinking_led_color;

PIO np_pio;
uint sm;

uint hour_to_index[LED_COUNT] = {
    24, // Means the Hole day
    23, 22, 21, 20, 15, 16, 17, 18, 19, 14, 13, 12, // AM
    11, 10, 5, 6, 7, 8, 9, 4, 3, 2, 1, 0 // PM
};

void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npWrite();

void LM_setHourColor(const uint hour, const uint8_t r, const uint8_t g, const uint8_t b) {
    npSetLED(hour_to_index[hour], r, g, b);
}

void LM_nextBlinkLed() {
    LM_setHourColor(blinking_hour, blinking_led_color.r, blinking_led_color.g, blinking_led_color.b);
    if (++blinking_hour >= LED_COUNT) {
        blinking_hour = 1;
    }
    blinking_led_on = true;
    blinking_led_color = leds[hour_to_index[blinking_hour]];
}

void LM_setBlinkLed(const uint hour) {
    if (hour > 24)
        return;
    LM_setHourColor(blinking_hour, blinking_led_color.r, blinking_led_color.g, blinking_led_color.b);    
    blinking_hour = hour;
    blinking_led_on = true;
    blinking_led_color = leds[hour_to_index[blinking_hour]];
}

void LM_update() {
    if (blinking_led_on)
        LM_setHourColor(blinking_hour, 0, 0, 0);
    else 
        LM_setHourColor(blinking_hour, blinking_led_color.r, blinking_led_color.g, blinking_led_color.b);
    blinking_led_on = !blinking_led_on;
    npWrite();
}

void LM_setup() {
    npInit(LED_PIN);
    LM_nextBlinkLed();
}



// *******************************************************************************************************
//                                      INTERNAL
// *******************************************************************************************************

void npInit(uint pin) {
    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
  
    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
      np_pio = pio1;
      sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }
  
    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
  
    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].r = 124;
        leds[i].g = 124;
        leds[i].b = 124;
    }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].r = r;
    leds[index].g = g;
    leds[index].b = b;
}

void npWrite() {
// Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].g);
        pio_sm_put_blocking(np_pio, sm, leds[i].r);
        pio_sm_put_blocking(np_pio, sm, leds[i].b);
    }
    //sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}



