#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"

void Dp_setup();
void Dp_drawString(char *str, int16_t x, int16_t y);
void Dp_drawUInt(uint16_t n, int16_t x, int16_t y, int8_t zero_fill);
void Dp_drawChar(char c, int16_t x, int16_t y );
void Dp_update();
void Dp_clear();

#endif