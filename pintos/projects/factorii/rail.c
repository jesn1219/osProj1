#include <stdio.h>
#include "projects/factorii/jesnk.h"





int robotArm_1_offset;
int robotArm_2_offset;
int pot_1_offset;
int pot_2_offset;



void rail_init(int* railStatus) {
//    railStatus = (int*)malloc(sizeof(int)*RAIL_COL*2+4);

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

void rail_update(int* railStatus) {
    // railStatus[1] = 1;
    for (int i =0 ; i < RAIL_COL; i++) {
    }
}




void rail_run(int* railStatus) {
    rail_init(railStatus);
    rail_update(railStatus);
}
