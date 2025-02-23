#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"

/**
 * @brief Initializes the display module.
 *
 * Sets up the I2C communication and initializes the SSD1306 OLED display.
 * Must be called once before using any other display functions.
 */
void Dp_setup();

/**
 * @brief Draws a string on the display at the specified position.
 *
 * Renders a text string on the OLED display starting from the given coordinates (x, y).
 *
 * @param str Pointer to the null-terminated string to be displayed.
 * @param x The x-coordinate (column) for the starting position of the string.
 * @param y The y-coordinate (row) for the starting position of the string.
 */
void Dp_drawString(char *str, int16_t x, int16_t y);

/**
 * @brief Draws an unsigned 16-bit integer on the display.
 *
 * Renders an unsigned 16-bit integer on the OLED display at the specified position.
 * Supports optional zero-padding to a specified width.
 *
 * @param n The unsigned 16-bit integer to be displayed.
 * @param x The x-coordinate (column) for the rightmost digit of the number. Digits are drawn to the left.
 * @param y The y-coordinate (row) for the digits.
 * @param zero_fill The number of digits to zero-fill from the left.
 *                  If zero_fill is 0 or negative, no zero-filling is performed.
 */
void Dp_drawUInt(uint16_t n, int16_t x, int16_t y, int8_t zero_fill);

/**
 * @brief Draws a single character on the display at the specified position.
 *
 * Renders a character on the OLED display at the given coordinates (x, y).
 *
 * @param c The character to be displayed.
 * @param x The x-coordinate (column) for the character.
 * @param y The y-coordinate (row) for the character.
 */
void Dp_drawChar(char c, int16_t x, int16_t y );

/**
 * @brief Updates the display, showing the buffered content.
 *
 * Sends the current content of the display buffer to the SSD1306 OLED display
 * to refresh the screen.  This must be called to make any drawing operations visible.
 */
void Dp_update();

/**
 * @brief Clears the display buffer.
 *
 * Fills the display buffer with black pixels, effectively clearing the display content
 * in memory.  `Dp_update()` must be called after `Dp_clear()` to update the actual display.
 */
void Dp_clear();

#endif