#ifndef TYPES_H
#define TYPES_H

#include "pico/stdlib.h"
#include <pico/aon_timer.h>

#define LED_COUNT 25
#define LED_PIN 7

#define LM_BLINK_PERIOD 1000
#define LM_NEXT_HOUR_PERIOD (10*1000)

#define MAX_EVENT_NAME_LEN 50

#define MAX_EVENT_NUM 100

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define DISPLAY_I2C_PORT i2c1
#define DISPLAY_I2C_SDA 14
#define DISPLAY_I2C_SCL 15
#define DISPLAY_I2C_FREQ (400*1000)
#define DISPLAY_I2C_ADDR _u(0x3C)

#define DISPLAY_CHAR_SIZE 8

#define MAX_SCREEN_STR_LEN 150

#define A_BUTTOM_PIN 5
#define B_BUTTOM_PIN 6
#define ANALOGIC_BUTTOM_PIN 22
#define ANALOGIC_X_PIN 27
#define ANALOGIC_Y_PIN 26
#define ADC_GPIO_INPUT(pin) ((pin)-26)

#define INPUT_BUFF_SIZE 16
#define INPUT_CHECK_ANALOG_PERIOD 425

#define BUZZER_PIN 21
#define BUZZER_FREQ 1760
#define BUZZER_DUTTY 4096
#define BUZZER_BEEP_TIME 1000

#define MAX_DAY_EVENTS 24

/**
 * @brief Structure representing a color in RGB format.
 *
 * This structure is used to define colors for the LED Matrix and potentially
 * for the screen display. It contains three 8-bit unsigned integers representing
 * the red, green, and blue components of the color.
 *
 * @var Color::r
 * @brief Red color component (0-255).
 *
 * @var Color::g
 * @brief Green color component (0-255).
 *
 * @var Color::b
 * @brief Blue color component (0-255).
 */
typedef struct Color {
    uint8_t r, g, b;
} Color;

/**
 * @brief Checks if two Color structures are equal.
 *
 * This function compares two Color structures and determines if their 
 * red, green, and blue components are identical.
 *
 * @param c1 Pointer to the first Color structure.
 * @param c2 Pointer to the second Color structure.
 * @return bool Returns true if the two Color structures are equal, false otherwise.
 */
bool Tp_ColorEq(const Color *c1, const Color *c2);

/**
 * @brief Type definition for an Event Index.
 *
 * This type is likely used to represent the index of an event within an array
 * or list of events.  Using `int8_t` suggests a limited number of events,
 * potentially up to 128 or 256 (depending on signed/unsigned interpretation
 * and usage in the code).
 */
typedef int8_t EventIndex_t;

/**
 * @brief Structure to represent a calendar event.
 *
 * This structure `Event` holds all the necessary information for a calendar event,
 * including its start and end times, name, color for display, alarm setting, and an index.
 */
typedef struct Event {
    char name[MAX_EVENT_NAME_LEN];      ///< Name of the event
    struct tm begin;                    ///< Start date and time of the event.
    struct tm end;                      ///< End date and time of the event.
    Color color;                        ///< Color associated with the event for display.
    bool alert;                         ///< Flag indicating if an alarm is set for the event.
    EventIndex_t index;                 ///< Unique index of the event for storage and retrieval.
} Event;

/**
 * @brief Checks if a given time falls within a specified time range.
 *
 * This function determines if a given time (tm1) is within the range defined
 * by a start time (begin) and an end time (end). The comparison is inclusive
 * of the begin time and exclusive of the end time ( [begin, end) ).
 *
 * @param tm1 Pointer to the time structure to be checked.
 * @param begin Pointer to the time structure representing the start of the range.
 * @param end Pointer to the time structure representing the end of the range.
 * @return bool Returns true if tm1 is within the time range [begin, end), false otherwise.
 */
bool Tp_timeIn(const struct tm *tm1,const struct tm *begin, const struct tm *end);
/**
 * @brief Checks if two time structures represent the same day.
 *
 * This function compares two time structures and determines if they fall on the
 * same day, ignoring the time of day. It likely compares year, month, and day of month.
 *
 * @param tm1 Pointer to the first time structure.
 * @param tm2 Pointer to the second time structure.
 * @return bool Returns true if tm1 and tm2 are on the same day, false otherwise.
 */
bool Tp_timeSameDay(const struct tm *tm1, const struct tm *tm2);
/**
 * @brief Checks if two time structures represent the same minute.
 *
 * This function compares two time structures and determines if they represent the
 * same minute, ignoring seconds and milliseconds. It likely compares year, month,
 * day of month, hour, and minute.
 *
 * @param tm1 Pointer to the first time structure.
 * @param tm2 Pointer to the second time structure.
 * @return bool Returns true if tm1 and tm2 are in the same minute, false otherwise.
 */
bool Tp_timeSameMin(const struct tm *tm1, const struct tm *tm2);

/**
 * @brief Enumeration of possible input types.
 *
 * This enumeration defines the different types of input that the calendar system
 * can receive from buttons and the analog joystick. It is used to represent
 * user interactions with the system.
 *
 * @var IP_NONE
 * @brief No input detected.
 *
 * @var IP_BUTTOM_A
 * @brief Button A press.
 *
 * @var IP_BUTTOM_B
 * @brief Button B press.
 *
 * @var IP_BUTTOM_ANALOG
 * @brief Analog button press (pressing down on the joystick).
 *
 * @var IP_ANALOG_UP
 * @brief Analog joystick moved upwards.
 *
 * @var IP_ANALOG_DOWN
 * @brief Analog joystick moved downwards.
 *
 * @var IP_ANALOG_LEFT
 * @brief Analog joystick moved to the left.
 *
 * @var IP_ANALOG_RIGHT
 * @brief Analog joystick moved to the right.
 */
typedef enum Input_t {
    IP_NONE,
    IP_BUTTOM_A,
    IP_BUTTOM_B,
    IP_BUTTOM_ANALOG,
    IP_ANALOG_UP,
    IP_ANALOG_DOWN,
    IP_ANALOG_LEFT,
    IP_ANALOG_RIGHT
} Input_t;

/**
 * @brief Enumeration of screen types or states.
 *
 * This enumeration defines the different screens or states that the calendar
 * application can display. It is used to manage the user interface and navigation.
 *
 * @var SC_MENU
 * @brief Main menu screen.
 *
 * @var SC_NEW_EVENT
 * @brief Screen for creating a new event.
 *
 * @var SC_NEW_EVENT_F
 * @brief Screen for (potentially) further details or finalization of a new event
 *        (the 'F' might stand for 'Finalize' or 'Further').
 */
typedef enum Sc_Type {
    SC_MENU,
    SC_NEW_EVENT,
    SC_NEW_EVENT_F
} Sc_Type;

/**
 * @brief Structure representing the screen display state.
 *
 * This structure holds information about what is currently displayed on the screen
 * and how it should be updated. It includes the text to be displayed, the current
 * screen type, and a function pointer for updating the screen based on events and input.
 *
 * @var Screen::text
 * @brief Text string to be displayed on the screen.
 * Maximum length is defined by `MAX_SCREEN_STR_LEN`.
 *
 * @var Screen::type
 * @brief Type of the current screen (e.g., menu, new event screen).
 *  Uses the `Sc_Type` enumeration.
 *
 * @var Screen::update
 * @brief Function pointer to a screen update function.
 *  This function is responsible for updating the `Screen` structure and potentially
 *  interacting with the `Event` data based on user `Input_t`.
 *  It takes a pointer to an `Event` and a pointer to `Input_t` as parameters and
 *  returns a `Sc_Type` value, likely indicating the next screen type to transition to.
 */
typedef struct Screen {
    char text[MAX_SCREEN_STR_LEN];
    Sc_Type type;
    Sc_Type (*update)(Event*, Input_t*);
} Screen;


#endif