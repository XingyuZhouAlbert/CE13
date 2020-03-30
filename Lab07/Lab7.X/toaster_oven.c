// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
#include "OledDriver.h"
#include "Oled.h"
#include "Buttons.h"
#include "Ascii.h"
#include "Adc.h"
// Microchip libraries
#include <xc.h>
#include <plib.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Declare any datatypes here ****
#define LONG_PRESS 5
#define BAKE_MODE 0
#define TOAST_MODE 1
#define BROIL_MODE 2
#define TIME_INPUT 0
#define TEMP_INPUT 1
// **** Define any module-level, global, or external variables here ****
char display [100];
static uint8_t butEvent = 0;
static int ledLightUp;
static char timeSelect;
static char tempSelect;
static uint16_t adcValue = 0;
static uint16_t min;
static uint16_t sec;

enum {
    ON = 1,
    OFF = 0
} OvenOF;

enum {
    RESET,
    START,
    COUNTDOWN,
    PENDING_SELECTOR_CHANGE,
    PENDING_RESET
} ovenState;

typedef struct timeEvents {
    uint8_t TwoHZ_TimerFlag;
    uint32_t freeRunningCounter;
} timeEvents;
timeEvents TE;

typedef struct ovenStateData {
    int cookingTimeLeft; //minute
    int cookTimeInit; //minute
    int temp; //degree fahrenheit 
    int cookMode;
    int ovenState;
    uint32_t butPressCount;
    int inputSelect;
    int oven_OF;
} ovenStateData;
ovenStateData OSD;
void updateDisplay(void);
void LedLightUp(void);
// Configuration Bit settings

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //Initialize everything
    LEDS_INIT();
    OledInit();
    ButtonsInit();
    AdcInit();
    while (1) {
        //constantly checking adc.
        switch (OSD.ovenState) {
        case RESET:
            OSD.inputSelect = TIME_INPUT;
            OSD.cookingTimeLeft = 1;
            OSD.temp = 350;
            OSD.oven_OF = OFF;
            LEDS_SET(0x00); //turn off all LEDS.
            updateDisplay();
            OSD.ovenState = START;
            break;
        case START:
            //potentiometer changed, Update time and temp loop back
            if (AdcChanged()) {
                adcValue = AdcRead() >> 2;
                if (OSD.inputSelect == TIME_INPUT) {
                    OSD.cookTimeInit = adcValue + 1;
                    OSD.cookingTimeLeft = OSD.cookTimeInit;
                } else if (OSD.inputSelect == TEMP_INPUT) {
                    OSD.temp = adcValue + 300;
                }
                updateDisplay();
                OSD.ovenState = START;
            }
            //stores running time and clear button event.
            if (butEvent & BUTTON_EVENT_3DOWN) {
                butEvent = 0;
                OSD.butPressCount = TE.freeRunningCounter;
                OSD.ovenState = PENDING_SELECTOR_CHANGE;
            }//reset timer, set cooktime clear btnevent.
            else if (butEvent & BUTTON_EVENT_4DOWN) {
                TIMER_2HZ_RESET();
                OSD.cookingTimeLeft = OSD.cookTimeInit;
                OSD.ovenState = COUNTDOWN;
                butEvent = 0;
                OSD.oven_OF = ON;
                LEDS_SET(0xFF);
                updateDisplay();
            }
            break;
        case PENDING_SELECTOR_CHANGE:
            if ((TE.freeRunningCounter - OSD.butPressCount) < (LONG_PRESS) && (butEvent & BUTTON_EVENT_3UP)) {
                if (OSD.cookMode == BAKE_MODE) {
                    OSD.cookMode = TOAST_MODE;
                    OSD.cookingTimeLeft = 1;
                    OSD.inputSelect = TIME_INPUT;
                    updateDisplay();
                } else if (OSD.cookMode == TOAST_MODE) {
                    OSD.cookMode = BROIL_MODE;
                    OSD.temp = 500;
                    OSD.inputSelect = TIME_INPUT;
                    updateDisplay();
                } else if (OSD.cookMode == BROIL_MODE) {
                    OSD.cookMode = BAKE_MODE;
                    OSD.cookingTimeLeft = 1;
                    OSD.temp = 350;
                    updateDisplay();
                }
                OSD.ovenState = START;
            } else if ((TE.freeRunningCounter - OSD.butPressCount) >= (LONG_PRESS)) {
                if (OSD.cookMode == BAKE_MODE) {
                    if (OSD.inputSelect == TIME_INPUT) {
                        OSD.inputSelect = TEMP_INPUT;
                    } else if (OSD.inputSelect == TEMP_INPUT) {
                        OSD.inputSelect = TIME_INPUT;
                    }
                }
                butEvent = 0;
                updateDisplay();
                OSD.ovenState = START;
            }
            break;
        case COUNTDOWN:
            if (butEvent & BUTTON_EVENT_4DOWN) {
                OSD.butPressCount = TE.freeRunningCounter;
                butEvent = 0;
                OSD.ovenState = PENDING_RESET;
            } else if ((TE.TwoHZ_TimerFlag == 1) && (OSD.cookingTimeLeft > 0)) {
                OSD.cookingTimeLeft--;
                updateDisplay();
                LedLightUp();
                TE.TwoHZ_TimerFlag = 0;
                OSD.ovenState = COUNTDOWN;
            } else if ((TE.TwoHZ_TimerFlag == 1) && (OSD.cookingTimeLeft == 0)) {
                updateDisplay();
                TE.TwoHZ_TimerFlag = 0;
                OSD.ovenState = RESET;
            }
            break;
        case PENDING_RESET:
            if (butEvent & BUTTON_EVENT_4UP) {
                butEvent = 0;
                OSD.ovenState = COUNTDOWN;
            } else if ((TE.TwoHZ_TimerFlag == 1) && (OSD.cookingTimeLeft > 0)) {
                OSD.cookingTimeLeft--;
                updateDisplay();
                TE.TwoHZ_TimerFlag = 0;
                OSD.ovenState = PENDING_RESET;
            } else if ((TE.freeRunningCounter - OSD.butPressCount) >= (LONG_PRESS)) {
                TE.TwoHZ_TimerFlag = 0;
                OSD.ovenState = RESET;
            }
            break;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{

    TE.TwoHZ_TimerFlag = 1;
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{

    TE.freeRunningCounter++;
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{

    butEvent = ButtonsCheckEvents();
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
}

//updateDisplay Function

void updateDisplay()
{
    min = OSD.cookingTimeLeft / 60;
    sec = OSD.cookingTimeLeft % 60;
    //decide the location of the selector in bake mode.
    if (OSD.inputSelect == TIME_INPUT) {
        timeSelect = '>';
        tempSelect = ' ';
    } else if (OSD.inputSelect == TEMP_INPUT) {
        timeSelect = ' ';
        tempSelect = '>';
    }
    //display the corresponding mode of the oven, NOT turning them on.
    if (OSD.cookMode == BAKE_MODE) {
        sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%c%s%2d%s%02d\n%s%c%s%d%c%c\n%c%c%c%c%c%c%c", '|', 0x02, 0x02, 0x02, 0x02, 0x02, "|  Mode: ", "Bake", " ",
                "|     |", timeSelect, " Time: ", min, ":", sec,
                "|-----|", tempSelect, " Temp: ", OSD.temp, 0xF8, 'F',
                '|', 0x04, 0x04, 0x04, 0x04, 0x04, '|');
    } else if (OSD.cookMode == TOAST_MODE) {
        sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%s%s%2d%s%02d\n%s\n%c%c%c%c%c%c%c", '|', 0x02, 0x02, 0x02, 0x02, 0x02, "|  Mode: ", "Toast", " ",
                "|     |", "  ", "Time: ", min, ":", sec,
                "|-----|",
                '|', 0x04, 0x04, 0x04, 0x04, 0x04, '|');
    } else if (OSD.cookMode == BROIL_MODE) {
        OSD.temp = 500;
        sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%s%s%2d%s%02d\n%s%s%s%d%c%c\n%c%c%c%c%c%c%c", '|', 0x02, 0x02, 0x02, 0x02, 0x02, "|  Mode: ", "Broil", " ",
                "|     |", " ", " Time: ", min, ":", sec,
                "|-----|", " ", " Temp: ", OSD.temp, 0xF8, 'F',
                '|', 0x04, 0x04, 0x04, 0x04, 0x04, '|');
    }
    //turn ON the oven
    if (OSD.oven_OF == ON) {
        if (OSD.cookMode == BAKE_MODE) {
            sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%c%s%2d%s%02d\n%s%c%s%d%c%c\n%c%c%c%c%c%c%c", '|', 0x01, 0x01, 0x01, 0x01, 0x01, "|  Mode: ", "Bake", " ",
                    "|     |", timeSelect, " Time: ", min, ":", sec,
                    "|-----|", tempSelect, " Temp: ", OSD.temp, 0xF8, 'F',
                    '|', 0x03, 0x03, 0x03, 0x03, 0x03, '|');
        } else if (OSD.cookMode == TOAST_MODE) {
            sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%s%s%2d%s%02d\n%s\n%c%c%c%c%c%c%c", '|', 0x02, 0x02, 0x02, 0x02, 0x02, "|  Mode: ", "Toast", " ",
                    "|     |", "  ", "Time: ", min, ":", sec,
                    "|-----|",
                    '|', 0x03, 0x03, 0x03, 0x03, 0x03, '|');
        } else if (OSD.cookMode == BROIL_MODE) {
            OSD.temp = 500;
            sprintf(display, "%c%c%c%c%c%c%s%s%s\n%s%s%s%2d%s%02d\n%s%s%s%d%c%c\n%c%c%c%c%c%c%c", '|', 0x01, 0x01, 0x01, 0x01, 0x01, "|  Mode: ", "Broil", " ",
                    "|     |", " ", " Time: ", min, ":", sec,
                    "|-----|", " ", " Temp: ", OSD.temp, 0xF8, 'F',
                    '|', 0x04, 0x04, 0x04, 0x04, 0x04, '|');
        }
    }
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(display);
    OledUpdate();
}

void LedLightUp()
{
    ledLightUp = (int) ((OSD.cookingTimeLeft * 8 / OSD.cookTimeInit)+1);
    if (ledLightUp == 8) {
        LEDS_SET(0xFF);
    } else if (ledLightUp == 7) {
        LEDS_SET(0xFE);
    } else if (ledLightUp == 6) {
        LEDS_SET(0XFC);
    } else if (ledLightUp == 5) {
        LEDS_SET(0xF8);
    } else if (ledLightUp == 4) {
        LEDS_SET(0xF0);
    } else if (ledLightUp == 3) {
        LEDS_SET(0xE0);
    } else if (ledLightUp == 2) {
        LEDS_SET(0xC0);
    } else if (ledLightUp == 1) {
        LEDS_SET(0x80);
    } else if (ledLightUp == 0) {
        LEDS_SET(0x00);
    }
}