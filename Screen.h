#ifndef SCREEN_H
#define SCREEN_H

#include "Display.h"
#include "Types.h"

void Sc_changeScreen(Screen *screen, Sc_Type type);

Sc_Type Sc_Menu_update(Event *event, Input_t *input_buf);

#endif