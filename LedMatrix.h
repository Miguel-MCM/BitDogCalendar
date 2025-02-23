#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"

#include "Types.h"

/**
 * @brief Sets the color of a specific hour LED on the LED Matrix.
 *
 * This function sets the RGB color of the LED corresponding to the given hour
 * on the LED Matrix. The mapping of hours to LEDs is handled internally.
 *
 * @param hour The hour (1-24) to set the color for.
 * @param r Red color component (0-255).
 * @param g Green color component (0-255).
 * @param b Blue color component (0-255).
 */
void LM_setHourColor(const uint hour,  const uint8_t r, const uint8_t g, const uint8_t b);

/**
 * @brief Advances the blinking LED to the next hour position on the LED Matrix.
 *
 * This function moves the currently blinking LED to the next hour in sequence
 * on the LED Matrix.  It's used to cycle through the hours for the blinking effect.
 */
void LM_nextBlinkLed();

/**
 * @brief Sets the blinking LED to a specific hour on the LED Matrix.
 *
 * This function sets the currently blinking LED to the LED corresponding to the
 * given hour.  It also stores the current color of that LED to maintain it when blinking is off.
 *
 * @param hour The hour (1-24) to set as the blinking LED.
 */
void LM_setBlinkLed(const uint hour);

/**
 * @brief Updates the LED Matrix display to produce the blinking effect.
 *
 * This function performs a single update cycle of the LED Matrix to create the
 * blinking effect for the currently set blinking LED. It toggles the blinking LED
 * on or off based on an internal state and then writes the updated LED data to the matrix.
 *
 * @note This function should be called periodically (e.g., using a timer) to achieve a continuous blinking effect.
 */
void LM_update();

/**
 * @brief Initializes the LED Matrix module.
 *
 * This function sets up the LED Matrix module, including initializing the PIO
 * (Programmable I/O) for controlling the LEDs, setting up the initial blinking LED,
 * and starting a repeating timer to drive the blinking effect using `LM_update`.
 */
void LM_setup();

#endif