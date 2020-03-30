/*Name: Xingyu Zhou
 collaborations: at this moment no.*/

#ifndef LEDS_H      //header guard
#define LEDS_H

#include <xc.h>
#include "BOARD.h"

//setting both TRISE and LATE registers to 0
#define LEDS_INIT() do{ \
TRISE = 0x00;\
LATE = 0x00;\
}while(0)

//return value
#define LEDS_GET() LATE 

//set the latch to register x
#define LEDS_SET(x) do{ \
LATE = (x);\
}while(0)
#endif