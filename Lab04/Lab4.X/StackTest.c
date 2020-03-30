// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****


// This code will unit test the Stack.c implementation. You should run at least two different tests
// on each function to be able to determine if it is working correctly. You will have to write these
// tests yourself.
struct Stack teststack;
struct Stack teststack2;
struct Stack teststack3;
struct Stack teststack4;
struct Stack teststack5;
int i, y, j, c;
int z = 0;
int testPassed = 0;
int totalPassed = 0;
float x;

int main()
{
    /*Create a nun empty stack.*/
    for (y = 0; y < STACK_SIZE - 6; y++) {
        z++;
        StackPush(&teststack2, z);
    }
    for (y = 0; y <= STACK_SIZE - 1; y++) {
        StackPush(&teststack4, 4);
    }
    StackInit(&teststack5);
    for (y = 0; y <= STACK_SIZE - 1; y++) {
        StackPush(&teststack5, 5);
    }
    BOARD_Init();
    /******************************** Your custom testing code goes below here ***********************/
    printf("Welcome to CMPE13 Stack Unit Testing for Xingyu Zhou/1517718.\r\n");
    //StackInit Test
    StackInit(&teststack);
    if (teststack.currentItemIndex == -1 && teststack.initialized == TRUE) {
        testPassed++;
    }
    StackInit(&teststack2);
    if (teststack2.currentItemIndex == -1 && teststack2.initialized == TRUE) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Passed (%d/2): StackInit()\n", testPassed);

    //StackIsEmpty Test.
    testPassed = 0;
    j = StackIsEmpty(&teststack);
    if (j == TRUE) {
        testPassed++;
    }
    j = StackIsEmpty(&teststack4);
    if (j == TRUE) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Teststack4 is not empty, test failed.\n");
    printf("Passed (%d/2) : StackIsEmpty()\n", testPassed);

    //StackPush Test
    testPassed = 0;
    StackInit(&teststack);
    StackPush(&teststack, 4.0);
    if (teststack.stackItems[teststack.currentItemIndex] == 4.0) {
        testPassed++;
    }
    StackInit(&teststack);
    StackPush(&teststack, 12.0);
    if (teststack.stackItems[teststack.currentItemIndex] == 12.0) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Passed (%d/2) : StackPush()\n", testPassed);

    // StackFull Test code
    testPassed = 0;
    c = StackIsFull(&teststack);
    if (c == FALSE) {
        testPassed++;
    }
    c = StackIsFull(&teststack5);
    if (c == TRUE) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Passed (%d/2) : StackIsFull()\n", testPassed);

    //StackPop Test Code;
    testPassed = 0;
    StackInit(&teststack);
    StackPush(&teststack, 7.0);
    StackPop(&teststack, &x);
    if (x == 7.0) {
        testPassed++;
    }

    StackInit(&teststack);
    StackPush(&teststack, 13.0);
    StackPop(&teststack, &x);
    if (x == 13.0) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Passed (%d/2) : StackIsPop()\n", testPassed);

    //StackGetSize test code
    testPassed = 0;
    StackInit(&teststack3);
    for (y = 0; y < STACK_SIZE - 1; y++) {
        StackPush(&teststack3, 4);
    }
    j = StackGetSize(&teststack3);
    if (j == 18) {
        testPassed++;
    }
    j = StackGetSize(&teststack4);
    if (j == SIZE_ERROR) {
        testPassed++;
    }
    if (testPassed == 2) {
        totalPassed++;
    }
    printf("Passed (%d/2) : StackGetSize()\n", testPassed);
    printf("%d out of 6 functions passed (%0.1f%%).\n", totalPassed, (double) (totalPassed / 6.0)*100);
    printf("I intentionally make StackEmpty failed the test to make sure it failed correctly, not a bug in my code.\n");
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}


