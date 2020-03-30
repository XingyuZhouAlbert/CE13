// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
double AbsoluteValue(double op1);
double FahrenheittoCelsius(double op2);
double Tangent(double op6);
double Average(double op4, double op5);
double CelsiustoFahrenheit(double op3);



// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    char input;
    char g; //garbage collector
    double op1;
    double op2;
    double op3;
    double op4;
    double op5;
    double op6;
    double op7;
    double op8;
    printf("Welcome to Albert's Calculator!\n");
    printf("Enter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
    while (1) {
        scanf("%c", &input);
        //this is the average part
        if (input == 'v') {
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op4, &g);
            printf("\nEnter the second operand: ");
            scanf("%f%c", &op5, &g);
            printf("\nResult of (%f v %f): %f", op4, op5, Average(op4, op5));
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == 'a') { //this is the absolute part
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op1, &g);
            printf("\nResults of |%f|: %f", op1, AbsoluteValue(op1));
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == 'c') { //F to C
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op2, &g);
            printf("\nResults of (%f deg->C): %f", op2, FahrenheittoCelsius(op2));
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == 'f') { //C to F
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op3, &g);
            printf("\nResults of (%f deg->F): %f", op3, CelsiustoFahrenheit(op3));
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == 't') { //Tangent 
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op6, &g);
            printf("\nResults of tan(%f):%f", op6, Tangent(op6));
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == '+') {
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op7, &g);
            printf("\nEnter the second operand: ");
            scanf("%f%c", &op8, &g);
            printf("\nResults of (%f + %f): %f", op7, op8, op7 + op8);
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == '-') {
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op7, &g);
            printf("\nEnter the second operand: ");
            scanf("%f%c", &op8, &g);
            printf("\nResults of (%f - %f): %f", op7, op8, op7 - op8);
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == '*') {
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op7, &g);
            printf("\nEnter the second operand: ");
            scanf("%f%c", &op8, &g);
            printf("\nResults of (%f * %f): %f", op7, op8, op7 * op8);
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        } else if (input == '/') {
            printf("\nEnter the first operand: ");
            scanf("%f%c", &op7, &g);
            printf("\nEnter the second operand: ");
            scanf("%f%c", &op8, &g);
            if (op8 != 0) {
                printf("\nResults of (%f / %f): %f", op7, op8, op7 / op8);
                printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
            } else {
                printf("\nError, Can't divide by 0");
                printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
            }
        } else { //Error
            printf("\nError, not a valid operator");
            printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        }
    }
}

/******************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 *****************************************************************************/

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double op1)
{
    if (op1 > 0) {
        return op1;
    } else if (op1 < 0) {
        return op1 * (-1);
    }
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheittoCelsius(double op2)
{
    return (op2 - 32.0) *(5.0 / 9.0);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiustoFahrenheit(double op3)
{
    return (op3 * (9.0 / 5.0)) + 32.0;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double op4, double op5)
{
    return ((op4 + op5) / 2);
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double Tangent(double op6)
{
    return tan((op6 / 180) * M_PI);
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
