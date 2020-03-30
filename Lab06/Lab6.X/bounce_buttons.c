// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
static uint8_t eventCheck;
static uint8_t ledLightUp;
static uint8_t swPosition;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    ButtonsInit();
    LEDS_INIT();
    while (1) {
        eventCheck = ButtonsCheckEvents();
        if (eventCheck != BUTTON_EVENT_NONE) { //check if the event flag is set.
            ledLightUp = LATE;
            swPosition = SWITCH_STATES();
            if (SWITCH_STATE_SW1 == 1 && (eventCheck & BUTTON_EVENT_1UP) == BUTTON_EVENT_1UP) {
                if ((ledLightUp & 0x03) == 0x03) { //turn off the led if it's already on.
                    ledLightUp = ledLightUp & 0xFC;
                } else {
                    ledLightUp ^= 0x03;
                }
            } else if (SWITCH_STATE_SW1 == 0 && (eventCheck & BUTTON_EVENT_1DOWN) == BUTTON_EVENT_1DOWN) {
                if ((ledLightUp & 0x03) == 0x03) {
                    ledLightUp = ledLightUp & 0xFC;
                } else {
                    ledLightUp ^= 0x03;
                }
            }
            if (SWITCH_STATE_SW2 == 2 && (eventCheck & BUTTON_EVENT_2UP) == BUTTON_EVENT_2UP) {
                //                            if ((ledLightUp & 0x0C) == 0x0C) {
                //                                ledLightUp = ledLightUp & 0xF3;
                //                            } else {
                ledLightUp ^= 0x0C;
                //                            }
            } else if (SWITCH_STATE_SW2 == 0 && (eventCheck & BUTTON_EVENT_2DOWN) == BUTTON_EVENT_2DOWN) {
                //                            if ((ledLightUp & 0x0C) == 0x0C) {
                //                                ledLightUp = ledLightUp & 0xF3;
                //                            } else {
                ledLightUp ^= 0x0C;
            }
            //                        }
            //            if (SWITCH_STATE_SW3 == 3 && (eventCheck & BUTTON_EVENT_3UP) == BUTTON_EVENT_3UP) {
            //                if ((ledLightUp & 0x30) == 0x30) {
            //                    ledLightUp = ledLightUp & 0xCF;
            //                } else {
            //                    ledLightUp ^= 0x30;
            //                }
            //            } else if (SWITCH_STATE_SW3 == 0 && (eventCheck & BUTTON_EVENT_3DOWN) == BUTTON_EVENT_3DOWN) {
            //                if ((ledLightUp & 0x30) == 0x30) {
            //                    ledLightUp = ledLightUp & 0xCF;
            //                } else {
            //                    ledLightUp ^= 0x30;
            //                }
            //            }
            //            if (SWITCH_STATE_SW4 == 4 && (eventCheck & BUTTON_EVENT_4UP) == BUTTON_EVENT_4UP) {
            //                if ((ledLightUp & 0x80) == 0x80) {
            //                    ledLightUp = ledLightUp & 0x3F;
            //                } else {
            //                    ledLightUp ^= 0x80;
            //                }
            //            } else if (SWITCH_STATE_SW4 == 0 && (eventCheck & BUTTON_EVENT_4DOWN) == BUTTON_EVENT_4DOWN) {
            //                if ((ledLightUp & 0x80) == 0x80) {
            //                    ledLightUp = ledLightUp & 0x3F;
            //                } else {
            //                    ledLightUp ^= 0x80;
            //                }
            //            }
            LEDS_SET(ledLightUp);
            eventCheck = 0;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    eventCheck = ButtonsCheckEvents();
}