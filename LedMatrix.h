#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"

#include "Types.h"

void LM_setHourColor(const uint hour,  const uint8_t r, const uint8_t g, const uint8_t b);
void LM_nextBlinkLed();
void LM_setBlinkLed(const uint hour);
void LM_update();

void LM_setup();

#endif