#ifndef BUTTONS_H
#define BUTTONS_H

/**
 * @file
 * This library provides an interface to the 4 pushbuttons on the Digilent Basic IO shield for their
 * Uno32 development board platform..
 *
 * This library relies on continuously polling the pins connected to the pushbuttons. It then
 * provides an event-based interface for use. The resultant output are either UpEvents or DownEvents
 * corresponding to whether the button has been pushed-down or released.
 *
 * Be aware that the ButtonsInit() function exists for configuring the appropriate pins on the PIC
 * processor and must be done before ButtonsCheckEvents() will work.
 */

#include <stdint.h>

// We rely on this file to define various macros for working with the hardware buttons.
#include "BOARD.h"

/**
 * Specify the number of samples that must be the same to be for a change in button state to be
 * recognized. So with this value at 4, it means that a button must be _---- for a EVENT_*DOWN
 * to be triggered. Similarly a -____ will trigger an EVENT_*UP. So this number corresponds to
 * the earliest an event will be detected after the button signal stabilizes due to debouncing.
 */
#define BUTTONS_DEBOUNCE_PERIOD 4

/**
 * This enum{} lists all of the possible button events that could occur. Each event constants were
 * chosen so that multiple button events can be recorded in a single call to ButtonsCheckEvents().
 * All possible event flags will also fit into a char datatype.
 *
 * In order to check for events occuring use bit-masking as the following code demonstrates:
 * 
 * uint8_t bEvent = ButtonsCheckEvents();
 * if (bEvent & BUTTON_EVENT_1DOWN) {
 *   // Button 1 was pressed
 * }
 */
typedef enum {
    BUTTON_EVENT_NONE = 0x00,
    BUTTON_EVENT_1UP = 0x01,
    BUTTON_EVENT_1DOWN = 0x02,
    BUTTON_EVENT_2UP = 0x04,
    BUTTON_EVENT_2DOWN = 0x08,
    BUTTON_EVENT_3UP = 0x10,
    BUTTON_EVENT_3DOWN = 0x20,
    BUTTON_EVENT_4UP = 0x40,
    BUTTON_EVENT_4DOWN = 0x80
} ButtonEventFlags;

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void);

/**
 * This function checks the button states and returns any events that have occured since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * This function also performs debouncing of the buttons following the guide to debouncing in the lab.
 * Each call to ButtonsCheckEvents should sample the buttons once.
 *
 * NOTE: This will not work properly without ButtonsInit() being called beforehand.
 * @return A bitwise-ORing of the constants in the ButtonEventFlags enum or BUTTON_EVENT_NONE if no
 *         event has occurred.
 */
uint8_t ButtonsCheckEvents(void);

#endif // BUTTONS_H
