// **** Include libraries here ****


/******************
 * 
 *HEAP SIZE : 10000
 *
 ******************/

// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"
#include "Ascii.h"
#include "Morse.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char display[500];
static char dotdashDisplay[500];
static char charDisplay[500];
static MorseEvent ME = 0;
static char topline[] = "                     "; //21 spaces for clearing the topline.
static char charOutput[100];
static char dotdashOutput[100];
static char decodedChar = '\0'; //store the decoded char for print.
static int dataCount = 0;
// **** Declare any function prototypes here ****
static void OledClearTop(void);
static void OledAppendTop(char data);
static void OledAppendBot(char data);

int main()
{
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    OledInit();
    if (MorseInit() == STANDARD_ERROR) {
        OledDrawString("Error!Initialization failed!\n");
        OledUpdate();
        FATAL_ERROR();
    }
    while (1) {
        
        if (ME != MORSE_EVENT_NONE) {
            if (ME == MORSE_EVENT_DOT) {
                OledClearTop();
                decodedChar = MorseDecode(MORSE_CHAR_DOT);
                OledAppendTop(decodedChar);
            } else if (ME == MORSE_EVENT_DASH) {
                OledClearTop();
                decodedChar = MorseDecode(MORSE_CHAR_DASH);
                OledAppendTop(decodedChar);
            } else if (ME == MORSE_EVENT_INTER_LETTER) {
                OledClearTop();
                decodedChar = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                if (decodedChar != STANDARD_ERROR) {
                    OledAppendBot(decodedChar);
                } else
                    OledAppendBot(MORSE_CHAR_END_OF_CHAR);
            }
            MorseDecode(MORSE_CHAR_DECODE_RESET);
        } else if (ME == MORSE_EVENT_INTER_WORD) {
            OledClearTop();
            decodedChar = MorseDecode(MORSE_CHAR_END_OF_CHAR);
            if (decodedChar != STANDARD_ERROR) {
                OledAppendBot(decodedChar);
            } else {
                OledAppendBot(MORSE_CHAR_END_OF_CHAR);
            }
            OledAppendBot(' ');
        }
        ME = MORSE_EVENT_NONE;
    }


    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    ME = MorseCheckEvents();
}

static void OledClearTop(void)
{
    sprintf(display, "%s", topline);
    OledDrawString(display);
    OledUpdate();
}

static void OledAppendTop(char data)
{
    if (dataCount <= 20) {
        dotdashOutput[dataCount] = data;
        dataCount++;
        sprintf(dotdashDisplay, "%s", dotdashOutput);
        OledDrawString(dotdashDisplay);
        OledUpdate();
    }
}

static void OledAppendBot(char data)
{
    if (dataCount > 20 && dataCount <= 62) {
        charOutput[dataCount] = data;
        dataCount++;
        sprintf(charDisplay, "%s", charOutput);
        OledDrawString(charDisplay);
        OledUpdate();
    }
}