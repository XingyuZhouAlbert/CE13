// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//CMPE13 Support Library
#include "BOARD.h"
#include "MatrixMath.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
float result [3][3] = {
    {0}
};
float result2 [3][3] = {
    {0}
}; //result2&3 only serves to MatrixMult, initialize the matrix.
float result3 [3][3] = {
    {0}
};
int TotalPassed = 0;
int i, j, k,y;
float sum = 0;
int flag = 1;
int counter = 0;
int x1 = 10;
int x2 = 5;
float det1 = 0;
float det2 = 195;
float mat0[3][3] = {
    {5, 2, 6},
    {5, 7, 6},
    {1, 8, 9}
};
float mat1 [3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float mat2 [3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float mat3 [3][3] = {
    {2, 3, 4},
    {5, 6, 7},
    {8, 9, 10}
};
float mat4 [3][3] = {
    {2, 3, 4},
    {5, 6, 7},
    {8, 9, 10}
};
float ExpResult1[3][3] = {
    {2, 4, 6},
    {8, 10, 12},
    {14, 16, 18}
}; //ExpResult1&2 are for MatrixAdd
float ExpResult2[3][3] = {
    {4, 6, 8},
    {10, 12, 14},
    {16, 18, 20}
};
float ExpResult3 = 15; //ExpResult3&4 are for MatrixTrace
float ExpResult4 = 18;
float ExpResult5[3][3] = {//ExpResult5&6 are for MatrixScalarMult
    {10, 20, 30},
    {40, 50, 60},
    {70, 80, 90}
};
float ExpResult6[3][3] = {//ExpResult5&6 are for MatrixScalarMult
    {5, 10, 15},
    {20, 25, 30},
    {35, 40, 45}
};
float ExpResult7[3][3] = {//ExpResult7&8 are for MatrixScalarAdd.
    {11, 12, 13},
    {14, 15, 16},
    {17, 18, 19}
};
float ExpResult8[3][3] = {
    {6, 7, 8},
    {9, 10, 11},
    {12, 13, 14}
};
float ExpResult9[3][3] = {//ExpResult9&10 are for MatrixTranspose
    {1, 4, 7},
    {2, 5, 8},
    {3, 6, 9}
};
float ExpResult10[3][3] = {
    {2, 5, 8},
    {3, 6, 9},
    {4, 7, 10}
};
float ExpResult11[3][3] = {
    {30, 36, 42},
    {66, 81, 96},
    {102, 126, 150}
};
float ExpResult12[3][3] = {
    {36, 42, 48},
    {81, 96, 111},
    {126, 150, 174}
};

int main()
{
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/

    //MatrixEquals test code
    counter = 0;
    y = MatrixEquals(mat1, mat2);
    if (y == TRUE) {
        counter++;
    }
    
    y = MatrixEquals(mat3, mat4);
    if (y == TRUE) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixEquals()\n", counter);
    if (counter == 2){
        TotalPassed++;
    }

    /*********************************************/
    //MatrixAdd test code
    counter = 0;
    MatrixAdd(mat1, mat2, result);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult1[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter = counter + 1;
    }
    MatrixAdd(mat3, mat4, result);
    flag = 1; //reset flag
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult2[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter = counter + 1;
    }
    printf("PASSED (%d/2): MatrixAdd()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /****************************************************************/

    //MatrixTrace test code
    counter = 0; //Initialize counter.
    sum = MatrixTrace(mat1);
    if (sum == ExpResult3) {
        counter++;
    }
    sum = MatrixTrace(mat3);
    if (sum == ExpResult4) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixTrace()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /********************************************************/

    //MatrixScalarMultply test code
    //Initializing both flag and counter
    MatrixScalarMultiply(x1, mat1, result);
    flag = 1;
    counter = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult5[i][j]) { //Test Failed
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++; //Test succeed
    }
    MatrixScalarMultiply(x2, mat1, result);
    flag = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult6[i][j]) { //Test Failed
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++; //Test succeed
    }
    printf("PASSED (%d/2): MatrixScalarMultiply()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /***********************************************************/
    //MatrixScalarAdd test code
    flag = 1;
    counter = 0;
    MatrixScalarAdd(x1, mat1, result);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult7[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    MatrixScalarAdd(x2, mat1, result);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult8[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixScalarAdd()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /******************************************************/
    //MatrixTranspose Test code
    flag = 1;
    counter = 0;
    MatrixTranspose(mat1, result);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult9[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    flag = 1;
    MatrixTranspose(mat3, result);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result[i][j] != ExpResult10[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixTranspose()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /***********************************************************/
    //MatrixDeterminant test code
    counter = 0;
    sum = MatrixDeterminant(mat1);
    if (sum == det1) {
        counter++;
    }
    sum = MatrixDeterminant(mat0);
    if (sum == det2) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixDeterminant()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }

    /********************************************************/
    //MatrixMultply test code
    flag = 1;
    counter = 0;
    MatrixMultiply(mat1, mat2, result2);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result2[i][j] != ExpResult11[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    //    //flag = 1;
    MatrixMultiply(mat1, mat3, result3);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (result3[i][j] != ExpResult12[i][j]) {
                flag = 0;
            }
        }
    }
    if (flag == 1) {
        counter++;
    }
    printf("PASSED (%d/2): MatrixMultiply()\n", counter);
    if (counter == 2) {
        TotalPassed++;
    }
    printf("%d out of 8 functions passed (%0.1f%%).\n", TotalPassed, (double) (TotalPassed /8.0)*100);

    //MatrixPrint() Testing Part
    printf(" Output of Matrix:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf(" |%0.3f| ", (double) mat1[i][j]);
        }
        printf("\n");
    }
    MatrixPrint(mat1);
    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}

