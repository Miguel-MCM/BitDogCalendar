#ifndef EVENTS_H
#define EVENTS_H

#include "Types.h"

EventIndex_t Ev_newEvent(const Event *event);
bool Ev_updateEvent(const Event *event, const EventIndex_t index);
bool Ev_getEvent(Event *event, const EventIndex_t index);
bool Ev_searchEvent(Event *event, const struct tm *time);

#endif