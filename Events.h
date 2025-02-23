#ifndef EVENTS_H
#define EVENTS_H

#include "Types.h"

/**
 * @brief Creates a new calendar event.
 *
 * Adds a new event to the event storage.
 *
 * @param event Pointer to the Event structure containing the event details.
 * @return EventIndex_t The index assigned to the newly created event.
 *         This index can be used to reference the event later for updates or retrieval.
 */
EventIndex_t Ev_newEvent(const Event *event);

/**
 * @brief Updates an existing calendar event.
 *
 * Modifies the details of an event already stored in the event storage.
 *
 * @param event Pointer to the Event structure containing the updated event details.
 * @param index The index of the event to be updated.
 * @return bool Returns `true` if the event at the given index was successfully updated,
 *         `false` if the index is invalid (e.g., out of range).
 */
bool Ev_updateEvent(const Event *event, const EventIndex_t index);

/**
 * @brief Retrieves a calendar event by its index.
 *
 * Fetches an event from the event storage based on its index.
 *
 * @param event Pointer to an Event structure where the retrieved event details will be stored.
 * @param index The index of the event to retrieve.
 * @return bool Returns `true` if an event was successfully retrieved and stored in `event`,
 *         `false` if the index is invalid (e.g., out of range).
 */
bool Ev_getEvent(Event *event, const EventIndex_t index);

/**
 * @brief Searches for an event that is active at a given time.
 *
 * Checks the event storage to find if any event is ongoing at the specified time.
 * "Ongoing" is determined by whether the given time falls within an event's begin and end times
 * using the `Tp_timeIn` function.
 *
 * @param event Pointer to an Event structure where the found event's details will be stored.
 * @param time Pointer to a `struct tm` representing the time to search for active events.
 * @return bool Returns `true` if an event is found that is active at the given time and
 *         its details are stored in `event`. Returns `false` if no active event is found.
 */
bool Ev_searchEvent(Event *event, const struct tm *time);

#endif