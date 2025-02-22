#ifndef CONTROL_H
#define CONTROL_H

#include "Types.h"

void Ctr_setupAll();
void Ctr_setup_aon_timer();
void Ctr_setupInput();
void Ctr_setupBuzzer();

void Ctr_buzzerBeep(uint32_t time);

void Ctr_addEvent(Event *event);

void Ctr_menuLoop();


#endif