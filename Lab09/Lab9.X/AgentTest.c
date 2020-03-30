// **** Include libraries here ****
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

Field myField, enemyField;
uint8_t passedCount = 0;
uint8_t totalPassed = 0;
// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();
    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    /******************
     AgentInit() Test
     ******************/
    printf("For AgentInit(),The actual output can be observed via OLED.\n");
    printf("I tested by myself and everything works fine :-).\n");

    /******************
     AgentGetStatus() Test 1
     ******************/
    myField.hugeBoatLives = 6;
    myField.largeBoatLives = 3;
    myField.mediumBoatLives = 0;
    myField.smallBoatLives = 0;
    uint8_t x = AgentGetStatus();
    if (x == 0x0C) {
        passedCount++;
    }

    /******************
     AgentGetStatus() Test 2
     ******************/
    myField.hugeBoatLives = 0;
    myField.largeBoatLives = 3;
    myField.mediumBoatLives = 0;
    myField.smallBoatLives = 1;
    x = AgentGetStatus();
    if (x == 0x05) {
        totalPassed++;
        passedCount++;
    }
    if (passedCount == 2) {
        printf("Passed(%d/2): AgentGetStatus()\n", passedCount);
    }

    /******************
    AgentGetEnemyStatus() Test 1
     ******************/
    passedCount = 0;
    enemyField.hugeBoatLives = 1;
    enemyField.largeBoatLives = 0;
    enemyField.mediumBoatLives = 0;
    enemyField.smallBoatLives = 0;
    uint8_t y = AgentGetEnemyStatus();
    if (y == 8) {
        passedCount++;
    }
    enemyField.hugeBoatLives = 1;
    enemyField.largeBoatLives = 1;
    enemyField.mediumBoatLives = 1;
    enemyField.smallBoatLives = 0;
    uint8_t z = AgentGetEnemyStatus();
    if (z == 14) {
        passedCount++;
        totalPassed++;
    }
    if (passedCount == 2) {
        printf("Passed(%d/2): AgentGetEnemyStatus()\n", passedCount);
    }

    printf("%d out of 2 functions passed (%0.1f%%).\n", totalPassed, (double) (passedCount / 2.0)*100);
    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}