/*Xingyu Zhou
 Collaboration Mingyu Ding*/
#include<stdint.h>
#include"BOARD.h"
#include "Buttons.h"

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

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
//variable declaration
uint8_t counter = 0;
static uint8_t firstSample, secondSample, thirdSample, fourthSample, fifthSample;

uint8_t ButtonsCheckEvents(void)
{
    uint8_t x = 0x00;
    firstSample = BUTTON_STATES();
    secondSample = BUTTON_STATES();
    thirdSample = BUTTON_STATES();
    fourthSample = BUTTON_STATES();
    fifthSample = BUTTON_STATES();
    if ((((firstSample) == (secondSample)) == ((thirdSample) == (fourthSample))) == fifthSample) { //making sure every sample is at the same state.
        if ((fifthSample & BUTTON_STATE_1) == BUTTON_EVENT_1DOWN) { //checking whether each button is pressed.
            x |= BUTTON_EVENT_1DOWN;
        } else if ((fifthSample & BUTTON_STATE_1) == BUTTON_EVENT_1UP) {
            x |= BUTTON_EVENT_1UP;
        }
        if ((fifthSample & BUTTON_STATE_2) == BUTTON_EVENT_2DOWN) {
            x |= BUTTON_EVENT_2DOWN;
        } else if ((fifthSample & BUTTON_STATE_2) == BUTTON_EVENT_2UP) {
            x |= BUTTON_EVENT_2UP;
        }
        if ((fifthSample & BUTTON_STATE_3) == BUTTON_EVENT_3DOWN) {
            x |= BUTTON_EVENT_3DOWN;
        } else if ((fifthSample & BUTTON_STATE_3) == BUTTON_EVENT_3UP) {
            x |= BUTTON_EVENT_3UP;
        }
        if ((fifthSample & BUTTON_STATE_4) == BUTTON_EVENT_4DOWN) {
            x |= BUTTON_EVENT_4DOWN;
        } else if ((fifthSample & BUTTON_STATE_4) == BUTTON_EVENT_4UP) {
            x |= BUTTON_EVENT_4UP;
        }
        if (x == 0x00) { //return NONE if nothing happens.
            x = BUTTON_EVENT_NONE;
            return x;
        }
    }
    return x;

    //    return 0;
}