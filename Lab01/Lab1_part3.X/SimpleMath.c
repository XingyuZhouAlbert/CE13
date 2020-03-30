/* ************************************************************************** */
/** SimpleMath

  @Company
    Company Name

  @File Name
    SimpleMath.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
//standard library
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "SimpleMath.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

int AddTwoNumbers (int y, int x){
    return x + y;
}

int IncrementByOne (int x){
    AddTwoNumbers (x,1);
    return AddTwoNumbers (x,1);
}