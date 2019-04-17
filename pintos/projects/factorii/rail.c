/*  Project name : AFS system
 *  file name : rail.c
 *  writer : JESOON KANG, 20170937
 *  file Description :
 *  this file include Functions for rail initializing
 *  */




#include <stdio.h>
#include "projects/factorii/jesnk.h"




/*  Function for initializing rail Status */
void rail_init(int* railStatus) {
    for (int i =0 ; i < RAIL_COL*2; i++) {
        railStatus[i] = 0;
    }
    railStatus[STAT_POT1OFFSET] = 0;
    railStatus[STAT_POT2OFFSET] = 0;
    railStatus[STAT_POT3OFFSET] = 0;
    railStatus[STAT_RBA1_1OFFSET] = 1;
    railStatus[STAT_RBA1_2OFFSET] = 1;
    railStatus[STAT_RBA1_3OFFSET] = 1;
    railStatus[STAT_RBA2_1OFFSET] = 1;
    railStatus[STAT_RBA2_2OFFSET] = 1;
    railStatus[STAT_RBA2_3OFFSET] = 1;
}


/*  rail Status array initializing */
void rail_run(int* railStatus) {
    rail_init(railStatus);
}
