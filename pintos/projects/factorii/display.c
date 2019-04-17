

#include <stdio.h>
#include "devices/timer.h"
#include "projects/factorii/jesnk.h"


#ifndef JESNK_DISP
#define JESNK_DISP



// Symbols
//
char divider;
char robotArm;
int rail_1[RAIL_COL];
int rail_2[RAIL_COL];

extern struct semaphore timer_disp;
extern struct semaphore timer_RA3;
extern struct semaphore timer_conveyer;
extern struct semaphore timer_RA5;
extern struct semaphore timer_RA6;
extern struct semaphore timer_RA3;
extern struct semaphore IF3_feIngIsEmpty;
extern struct semaphore IF3_coIngIsEmpty;

extern int feIng;
extern int coFeIng;
extern int testSign;
extern int NUM_FE_ORE;
extern int NUM_CO_ORE;
extern int NUM_FE_ING;
extern int NUM_CO_ING;
extern int NUM_CO_NEED;
extern int NUM_FE_NEED;


int tmp_jesnk;
void display_init(char* display) {
    for (int i = 0; i < RAIL_COL; i++ ) {
        display[i] = 'X';
        display[RAIL_COL*2+i] = 'X';
        display[RAIL_COL*4+i] = 'X';
    }
}


void display_setting(int* railStatus, char* display) {
    for (int i = 0; i <= 2; i ++) {
        if (*(railStatus+i) == 1) {
            display[RAIL1OFFSET+i] = 'o';
        } else {
            display[RAIL1OFFSET+i] = ' ';
        }
    }
    for (int i = 6; i <= 8; i++) {
        if (*(railStatus+i)) {
            display[RAIL1OFFSET+i] = 'i';
        } else {
            display[RAIL1OFFSET+i] = ' ';
        }
    } 
    
    
    for (int i = 0; i <= 2; i ++) {
        if (*(railStatus+RAIL_COL+i) == 1) {
            display[RAIL2OFFSET+i] = 'o';
        } else {
            display[RAIL2OFFSET+i] = ' ';
        }
    }
    for (int i = 6; i <= 8; i++) {
        if (*(railStatus+RAIL_COL+i)) {
            display[RAIL2OFFSET+i] = 'i';
        } else {
            display[RAIL2OFFSET+i] = ' ';
        }
    } 

    // POT DISPLAY
    //
    switch (*(railStatus+4)) {
        case 2 : display[RAIL1OFFSET + DIS_POT_1_OFFSET] = 'i'; break;
        case 1 : display[RAIL1OFFSET + DIS_POT_1_OFFSET] = 'o'; break;
        case 0 : display[RAIL1OFFSET + DIS_POT_1_OFFSET] = '-'; break;
        case -1 : display[RAIL1OFFSET + DIS_POT_1_OFFSET] = '-'; break;
        
    }

    switch (*(railStatus+RAIL_COL*+1+4)) {
        case 2 : display[RAIL2OFFSET + DIS_POT_1_OFFSET] = 'i'; break;
        case 1 : display[RAIL2OFFSET + DIS_POT_1_OFFSET] = 'o'; break;
        case 0 : display[RAIL2OFFSET + DIS_POT_1_OFFSET] = '-'; break;
        case -1 : display[RAIL2OFFSET + DIS_POT_1_OFFSET] = '-'; break;
    }

    switch (*(railStatus+STAT_POT3OFFSET)) {
        case 2 : display[RAIL1OFFSET*2+DIS_POT_2_OFFSET] = '*'; break;
        case 1 : display[RAIL1OFFSET*2+DIS_POT_2_OFFSET] = 'i'; break;
        case 0 : display[RAIL1OFFSET*2+DIS_POT_2_OFFSET] = '-'; break;
    }

    if (*(railStatus+STAT_RBA1_1OFFSET)) {
        display[RAIL1OFFSET + DIS_RBA_1_OFFSET] = '@';
    }
    if (*(railStatus+STAT_RBA1_2OFFSET)) {
        display[RAIL1OFFSET + DIS_RBA_2_OFFSET] = '@';
    }
    if (*(railStatus+STAT_RBA2_1OFFSET)) {
        display[RAIL2OFFSET + DIS_RBA_1_OFFSET] = '@';
    }
    if (*(railStatus+STAT_RBA2_2OFFSET)) {
        display[RAIL2OFFSET + DIS_RBA_2_OFFSET] = '@';
    } 
   
    if (*(railStatus+STAT_RBA1_3OFFSET)) {
        display[RAIL1OFFSET + DIS_RBA_3_OFFSET] = '@';
    } 
    if (*(railStatus+STAT_RBA2_3OFFSET)) {
        display[RAIL2OFFSET + DIS_RBA_3_OFFSET] = '@';
    } 
}

int* globalRailStatus;
void display_show(char* display) {
   for (int i = 0; i < RAIL_ROW; i++) {
       for (int j = 0; j < RAIL_COL; j++) {
           printf("%c",display[i*RAIL_COL + j]);
       }
       printf("\n");


   }
   for (int i = 0; i < STAT_RAIL2OFFSET; i++) {
      printf("%d;",globalRailStatus[i]);
   }
   printf("\n");
   for (int i = STAT_RAIL2OFFSET; i < 21; i++) {
       printf("%d;",globalRailStatus[i]);
   }

   printf("\n");

   printf("LEFT : FeOre=%d, CoOre=%d, FeIng=%d, CoIng=%d\n",NUM_FE_ORE,NUM_CO_ORE,NUM_FE_ING,NUM_CO_ING);
   printf("MADE : CoFe=%d\n",coFeIng);
   printf("feisempty : %d, coIsEmpty: %d, \n",IF3_feIngIsEmpty.value, IF3_coIngIsEmpty.value );

   printf("testSign : %d\n",testSign);
}


void display_run(int* railStatus) {
    globalRailStatus = railStatus;
    char* display  = (char*)malloc(sizeof(char)*RAIL_ROW*RAIL_COL); 
    tmp_jesnk = 0;
    display_init(display);

    printf("%c\n",display); 
    while(true) { 
        sema_down(&timer_disp);
        printf("\033[2J");
        printf("Hello %d\n",tmp_jesnk);
        tmp_jesnk++;
        display_setting(railStatus,display); 
        display_show(display);
        sema_up(&timer_RA5);
        sema_up(&timer_RA6);
    }
}














#endif // JESNK_DISP
