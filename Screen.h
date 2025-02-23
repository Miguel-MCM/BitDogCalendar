#ifndef SCREEN_H
#define SCREEN_H

#include "Display.h"
#include "Types.h"

/**
 * @brief Changes the current screen of the application.
 *
 * This function switches the application's display to a new screen type.
 * It clears the display and sets the `update` function pointer within the
 * provided `Screen` structure to the appropriate update function for the
 * specified screen type.
 *
 * @param screen Pointer to the Screen structure whose `update` function will be changed.
 * @param type The new screen type to switch to, defined by the `Sc_Type` enumeration.
 */
void Sc_changeScreen(Screen *screen, Sc_Type type);

/**
 * @brief Updates the Menu screen display and handles user input in the Menu screen.
 *
 * This function is responsible for rendering the main Menu screen on the display
 * and processing user inputs (buttons and analog joystick) when the application
 * is in the Menu screen state (`SC_MENU`). It manages menu navigation and action
 * selection based on user input.
 *
 * @param event Pointer to the current Event structure (used to pass current date_time information).
 * @param input_buf Array of Input_t values representing user inputs to be processed.
 * @return Sc_Type Returns the next screen type based on user input and menu actions.
 *         Typically returns `SC_MENU` to remain in the menu, or another `Sc_Type`
 *         to transition to a different screen (e.g., `SC_NEW_EVENT`).
 */
Sc_Type Sc_Menu_update(Event *event, Input_t *input_buf);

/**
 * @brief Updates the New Event screen display and handles user input for creating a new event.
 *
 * This function manages the display and user interaction for the "New Event" screen
 * (`SC_NEW_EVENT`). It handles input for setting event details like time, name, alarm, and color.
 * It progresses through different stages of event creation based on user input and
 * returns the next screen type.
 *
 * @param event Pointer to the Event structure being created/modified in the New Event screen.
 * @param input_buf Array of Input_t values representing user inputs to be processed.
 * @return Sc_Type Returns the next screen type based on user input and event creation stage.
 *         Typically returns `SC_NEW_EVENT` to remain in the New Event screen,
 *         or `SC_MENU` to go back to the menu, or `SC_NEW_EVENT_F` when a new event is finalized.
 */
Sc_Type Sc_New_Event_update(Event *event, Input_t *input_buf);

#endif