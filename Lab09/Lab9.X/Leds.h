//Mingyu Ding
//miding@ucsc.edu
//collaborator: Albert Zhou

#ifndef LEDS_H
#define	LEDS_H

#include <xc.h>
#include "BOARD.h"

//define LEDS_INIT
#define LEDS_INIT()do{\
    TRISE &= 0x00;\
    LATE  &= 0x00;\
}while (0)
//define LEDS_GET
#define LEDS_GET() LATE

//define LEDS_SET
#define LEDS_SET(x);do{\
    LATE = (x);\
}while (0)


#endif	

