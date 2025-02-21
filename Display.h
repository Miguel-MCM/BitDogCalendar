#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"

void Dp_setup();
void Dp_drawString(char *str, int16_t x, int16_t y);
void Dp_update();

#endif