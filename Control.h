#ifndef CONTROL_H
#define CONTROL_H

#include "Types.h"

/**
 * @brief Initializes all calendar system modules.
 *
 * Orchestrates the setup of all hardware and software components
 * required for the calendar to function.  Call once at program start.
 *
 * @note Must be called before any other functions in the Control module.
 */
void Ctr_setupAll();

/**
 * @brief Initializes the Always-On Timer (AON) for timekeeping.
 *
 * Configures the Pico's AON timer to maintain calendar time.
 * Called internally by `Ctr_setupAll()`.
 */
void Ctr_setup_aon_timer();

/**
 * @brief Initializes the input system (buttons, analog joystick).
 *
 * Sets up GPIOs for button input and ADC for analog joystick.
 * Configures interrupts for button presses and a timer for analog input.
 * Called internally by `Ctr_setupAll()`.
 */
void Ctr_setupInput();

/**
 * @brief Initializes the buzzer for audio alerts.
 *
 * Configures PWM to control the buzzer.
 * Called internally by `Ctr_setupAll()`.
 */
void Ctr_setupBuzzer();

/**
 * @brief Produces a beep sound using the buzzer for a given duration.
 *
 * Activates the buzzer for 'time' milliseconds.
 *
 * @param time Duration of the beep in milliseconds.
 */
void Ctr_buzzerBeep(uint32_t time);

/**
 * @brief Adds a new calendar event.
 *
 * Stores the given event and updates the display if the event is for the current day.
 *
 * @param event Pointer to the Event structure to add.
 */
void Ctr_addEvent(Event *event);

/**
 * @brief Runs the main calendar menu loop.
 *
 * Contains the primary program loop for user interaction, screen updates,
 * and event handling.  Call repeatedly in the main program.
 */
void Ctr_menuLoop();


#endif