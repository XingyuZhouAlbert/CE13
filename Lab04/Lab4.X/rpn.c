// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
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
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#define STRING_SIZE 60
#ifndef LAB4_TESTING
float op1;
float op2;

int main() {
    BOARD_Init();
#else

int their_main(void) {
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/
    printf("Welcome to Albert's RPN calculator.\n");
    while (1) {
        printf("Enter floats and + - / * in RPN format: \n");
        char inputString[STRING_SIZE] = {0};
        fgets(inputString, STRING_SIZE + 2, stdin); //Read the value
        char *tokenPointer = strtok(inputString, " "); //Aim the pointer to space.

        while (tokenPointer != NULL) {
            tokenPointer = strtok(NULL, " ");
        }
//        op1 = atof(inputString);
//        printf("The number you inputed is: %.2f.\n", (double) op1);
    }
    //        }
    //    }
    /*************************************************************************************************/
    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence) {
    return 0;
}



