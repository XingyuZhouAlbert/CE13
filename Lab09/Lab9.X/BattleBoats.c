// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "CircularBuffer.h"
#include "Leds.h"
#include "Oled.h"
#include "Buttons.h"
#include "Protocol.h"
#include "Uart1.h"
#include "Field.h"
#include "OledDriver.h"
#include "FieldOled.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static uint32_t counter;
static uint8_t buttonEvents;

// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();

    // Set up UART1 for output.
    Uart1Init(UART_BAUD_RATE);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a 10ms timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Disable buffering on stdout
    setbuf(stdout, NULL);

    ButtonsInit();

    LEDS_INIT();

    OledInit();

    // Prompt the user to start the game and block until the first character press.
    OledDrawString("Press BTN4 to start.");
    OledUpdate();
    while ((buttonEvents & BUTTON_EVENT_4UP) == 0);

    // The first part of our seed is a hash of the compilation time string. The lowest-8 bits
    // are xor'd from the first-half of the string and the highest 8-bits are xor'd from the
    // second-half of the string.
    char seed1[] = __TIME__;
    int seed1_len = strlen(seed1);
    int firstHalf = seed1_len / 2;
    uint16_t seed2 = 0;
    int i;
    for (i = 0; i < seed1_len; ++i) {
        seed2 ^= seed1[i] << ((i < firstHalf) ? 0 : 8);
    }

    // Now we hash-in the time since first user input (which, as a 32-bit number, is split
    // and each half is hashed in.
    srand(seed2 ^ ((uint16_t) (counter >> 16)) ^ (uint16_t) (counter));

    // Initialize the human agent.
    AgentInit();

    while (TRUE) {

        // Check to see that the agent is still alive, updating the LED display as needed.
        uint8_t agentLives = AgentGetStatus();
        LEDS_SET(agentLives);

        // Also check if the enemy is still alive. If not, flash all the LEDs for this agent.
        uint8_t enemyLives = AgentGetEnemyStatus();
        if (enemyLives == 0) {
            // Otherwise blink the LEDs signifying the winner. We just turn off all LEDs here,
            // because they'll be turned back on at the beginning of the event loop. This creates
            // an approximately 1Hz blinking pattern.
            uint32_t i;
            for (i = 0; i < 5000000; ++i);
            LEDS_SET(0);
            for (i = 0; i < 5000000; ++i);
        }// Now only if the enemy is still alive do we run the main event loop.
        else if (agentLives > 0) {
            // Grab an ASCII byte from the UART to pass to the agent. 0 indicates no new data.
            uint8_t inData = 0;
            Uart1ReadByte(&inData);

            // And then output this agents response
            char outData[64];
            int outDataLength = AgentRun((char) inData, outData);
            if (outDataLength > 0) {
                Uart1WriteData(outData, outDataLength);
            }
        }
    }
}

/**
 * This is the interrupt for the Timer2 peripheral. It just keeps incrementing a counter used to
 * track the time until the first user input.
 */
void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Increment a counter to see the srand() function.
    counter++;

    // Also check for any button events
    buttonEvents = ButtonsCheckEvents();
}