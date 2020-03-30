#include "BOARD.h"
#include "MatrixMath.h"
#include <stdio.h>

#define TRUE 1;
#define FALSE 0;


int i, j, k;

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result [i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result [i][j] = mat [i][j] * x;
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] + x;
        }
    }
}

float MatrixTrace(float mat[3][3])
{
    float sum = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                sum += mat [i][j];
            }
        }
    }
    return sum;
}

/*I don't quite understand how to implement the method provided in the lab manual
 so I'm doing it with a dumber way*/
float MatrixDeterminant(float mat[3][3])
{
    return mat[0][0]*((mat[1][1] * mat[2][2])-(mat[2][1] * mat[1][2])) - mat[0][1]*((mat[1][0] * mat[2][2])-(mat[2][0] * mat[1][2])) + mat[0][2]*((mat[1][0] * mat[2][1])-(mat[2][0] * mat[1][1]));
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void MatrixTranspose(float mat[3][3], float result[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[j][i] = mat [i][j];
        }
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (mat1[i][j] - mat2[i][j] > FP_DELTA || mat1[i][j] - mat2[i][j] < -FP_DELTA) {
                return FALSE; 
            }
        }
    }
    return TRUE;
}

void MatrixPrint(float mat[3][3])
{
    printf(" Expected output of Matrix:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf(" |%0.3f| ", (double) mat[i][j]);
        }
        printf("\n");
    }
}
