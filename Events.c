#include "Events.h"

Event events[MAX_EVENT_NUM];
EventIndex_t current_event_id = 0;
EventIndex_t events_num = 0;

EventIndex_t Ev_newEvent(const Event *event) {
    if (current_event_id == MAX_EVENT_NUM)
        current_event_id = 0;
    events[current_event_id] = *event;
    events[current_event_id].index = current_event_id;
    current_event_id++;
    if (++events_num > MAX_EVENT_NUM)
        events_num = MAX_EVENT_NUM;
}

bool Ev_updateEvent(const Event *event, const EventIndex_t index) {
    if (index >= events_num)
        return false;
    events[index] = *event;
    events[index].index = index;
    return true;
}

bool Ev_getEvent(Event *event, const EventIndex_t index) {
    if (index >= events_num)
        return false;
    *event = events[index];
}

bool Ev_searchEvent(Event *event, const struct tm *time) {
    EventIndex_t i;
    Event *current;
    for (i=0; i<events_num; i++) {
        current = &events[i];
        if (Tp_timeIn(time, &(current->begin), &(current->end))) {
            *event = *current;
            return true;
        }
    }
    return false;
}

