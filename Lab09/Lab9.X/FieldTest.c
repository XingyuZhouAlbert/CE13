// **** Include libraries here ****

//Xingyu Zhou
//Collaborate with Jimmy Chen.

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
Field myField;
Field enemyField;
FieldPosition myPos;
int passedCount, totalPassed;
// **** Declare any function prototypes here ****

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    //FieldInit() Test 1
    FieldInit(&myField, myPos);
    if (myField.hugeBoatLives == 6) {
        totalPassed++;
    }
    //FieldInit() Test 2
    myField.largeBoatLives = 20;
    FieldInit(&myField, myPos);
    if (myField.largeBoatLives == 5) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldInit()\n", totalPassed);
    }
    /************************************************************************************/
    //FieldAt() Test 1
    totalPassed = 0;
    myPos = FieldAt(&myField, 3, 5);
    if (myPos == myField.field[3][5]) {
        totalPassed++;
    }
    //FieldAt() Test2
    myPos = FieldAt(&myField, 6, 10);
    if (myPos == myField.field[6][10]) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldAt()\n", totalPassed);
    }
    /************************************************************************************/
    //FieldSetLocation() Test 1
    totalPassed = 0;
    FieldSetLocation(&myField, 4, 2, myPos);
    if (myPos == 0) {
        totalPassed++;
    }
    //FieldTestSetLocation() Test 2
    myPos = FieldAt(&myField, 3, 2);
    FieldSetLocation(&myField, 5, 8, myPos);
    if (myPos == myField.field[3][2]) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldSetLocation()\n", totalPassed);
    }
    /************************************************************************************/
    //FieldAddBoat() Test 1
    totalPassed = 0;
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    if ((myField.field[0][0] == FIELD_POSITION_SMALL_BOAT)&& (myField.field[0][1] == FIELD_POSITION_SMALL_BOAT) && (myField.field[0][2] == FIELD_POSITION_SMALL_BOAT)) {
        totalPassed++;
    }
    //FieldAddBoat() Test 2
    FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_MEDIUM);
    if ((myField.field[0][6] == FIELD_POSITION_MEDIUM_BOAT)&& (myField.field[1][6] == FIELD_POSITION_MEDIUM_BOAT) && (myField.field[2][6] == FIELD_POSITION_MEDIUM_BOAT)&& (myField.field[3][6] == FIELD_POSITION_MEDIUM_BOAT)) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldAddBoat()\n", totalPassed);
    }
    /************************************************************************************/
    //FieldRegisterEnemyAttack() Test 1
    totalPassed = 0;
    GuessData gData;
    gData.col = 0;
    gData.row = 0;
    FieldRegisterEnemyAttack(&myField, &gData);
    gData.col = 1;
    FieldRegisterEnemyAttack(&myField, &gData);
    gData.col = 2;
    FieldRegisterEnemyAttack(&myField, &gData);
    if (gData.hit == HIT_SUNK_SMALL_BOAT) {
        totalPassed++;
    }
    //FieldRegisterEnemyAttack() Test 2
    gData.row = 0;
    gData.col = 6;
    FieldRegisterEnemyAttack(&myField, &gData);
    if (myField.mediumBoatLives == 3) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldRegisterEnemyAttack()\n", totalPassed);
    }
    /************************************************************************************/
    //FieldUpdateKnowledge() Test 1
    totalPassed = 0;
    gData.row = 0;
    gData.col = 6;
    myPos = FieldUpdateKnowledge(&myField, &gData);
    if (myPos == myField.field[0][6]) {
        totalPassed++;
    }
    gData.hit = HIT_SUNK_MEDIUM_BOAT;
    FieldUpdateKnowledge(&myField,&gData);
    if(myField.mediumBoatLives == 0){
        totalPassed++;
    }
    if(totalPassed == 2){
        passedCount ++;
        printf("Passed(%d/2): FieldUpdateKnowledge()\n", totalPassed);
    }

    /************************************************************************************/
    //FieldGetBoatStates() Test 1
    uint8_t boatStates = 0;
    totalPassed = 0;
    myField.smallBoatLives = 0;
    myField.mediumBoatLives = 0;
    boatStates = FieldGetBoatStates(&myField);
    if (boatStates == 0x0C) {
        totalPassed++;
    }
    //FieldBoatStates() Test 2
    myField.smallBoatLives = 3;
    myField.largeBoatLives = 2;
    myField.hugeBoatLives = 0;
    myField.mediumBoatLives = 0;
    boatStates = FieldGetBoatStates(&myField);
    if (boatStates == 0x05) {
        totalPassed++;
    }
    if (totalPassed == 2) {
        passedCount++;
        printf("Passed(%d/2): FieldGetBoatStates()\n", totalPassed);
    }
    /************************************************************************************/
    //print out the results
    printf("%d out of 7 functions passed (%0.1f%%).\n", passedCount, (double) (passedCount / 7.0)*100);
    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}