/*
 * Project name : AFS system
 * file name : factorii.c
 * writer : JESOON KANG, 20170937
 * last modified date : 2019. 04.17
 * description :
 * main AFS works implements in this file.
 * this file includess Functions for each Robot Arms, IFs, and other checking Functions,
 * and implemented run_factorii function.
 *  */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "projects/factorii/display.c"
#include "projects/factorii/rail.c"
#include "projects/factorii/factorii.h"




/*  this Function runs for carrying the ores & ings.  
 *  conveyer Function for conveyor thread*/
void run_conveyer() {
    while(true) {
        sema_down(&timer_conveyer);

      /*  chage tmp values in IF. */  
        if (railStatus[4] == -1) {
            railStatus[4] = 0;
        }
        if (railStatus[STAT_RAIL2OFFSET+4] == -1) {
            railStatus[STAT_RAIL2OFFSET+4] = 0;
        }


        /*  carrying ores in rail 1 */
        if (!railStatus[2]) {
            railStatus[2] = railStatus[1];
            railStatus[1] = railStatus[0];
            railStatus[0] = NUM_FE_ORE_STORED > 0 ? 1 : 0 ; 
            if (NUM_FE_ORE_STORED > 0) {
                NUM_FE_ORE_STORED--;
            }
            if (railStatus[2]) {
                sema_up(&RA1);
            }
        }

        /*  carrying ores in rail 2 */
        if (!railStatus[STAT_RAIL2OFFSET+2]) {
            railStatus[STAT_RAIL2OFFSET+2] = railStatus[STAT_RAIL2OFFSET+1];
            railStatus[STAT_RAIL2OFFSET+1] = railStatus[STAT_RAIL2OFFSET];
            railStatus[STAT_RAIL2OFFSET] = NUM_CO_ORE_STORED > 0 ? 1 : 0;   
            if (NUM_CO_ORE_STORED > 0) {
                NUM_CO_ORE_STORED--;
            }

            if (railStatus[STAT_RAIL2OFFSET+2]) {
                sema_up(&RA2);
            } 
        }

        /*  carrying ings in rail 1 */ 
        if (!railStatus[8]) {
            railStatus[8] = railStatus[7];
            railStatus[7] = railStatus[6] == 1 ? 1 : 0 ;
            if (railStatus[6] == -1) {
                railStatus[6] = 1;
            } else {
                railStatus[6] = 0;
            }
            if (railStatus[8]) {
                sema_up(&RA5);
            }
        }

        /*  carrying ings in rail 2 */
        if (!railStatus[STAT_RAIL2OFFSET+8]) {
            railStatus[STAT_RAIL2OFFSET+8] = railStatus[STAT_RAIL2OFFSET+7];
            railStatus[STAT_RAIL2OFFSET+7] = railStatus[STAT_RAIL2OFFSET+6] == 1 ? 1 : 0 ;
            if (railStatus[STAT_RAIL2OFFSET+6] == -1) {
                railStatus[STAT_RAIL2OFFSET+6] = 1;
            } else {
                railStatus[STAT_RAIL2OFFSET+6] = 0;
            }
            if (railStatus[STAT_RAIL2OFFSET + 8]) {
                sema_up(&RA6);
            }
        }

        /*  calculate ings number */
        NUM_FE_ING = 0; 
        NUM_CO_ING = 0;
        for (int i = 6; i <=8; i++) {
            if (railStatus[i] == 1) {
                NUM_FE_ING++;
            }
            if( railStatus[STAT_RAIL2OFFSET+i] == 1) {
                NUM_CO_ING++;
            }
        }
        if (railStatus[4] == 2) {
            NUM_FE_ING++;
        }
        if (railStatus[STAT_RAIL2OFFSET+4] == 2) {
            NUM_CO_ING++;
        }



        /*  check terminate condition */
        if ( (NUM_FE_ORE + NUM_FE_ING < IF3_feIngIsEmpty.value) || (NUM_CO_ORE + NUM_CO_ING < IF3_coIngIsEmpty.value)) {
            if (railStatus[STAT_POT3OFFSET] == 0)  {
                /*  terminate Flag up */
                terminateFlag = 1; 
            }

        }

        /* terminate AFS program . */
        if (terminateFlag) {
            terminateCount++;
            if (terminateCount >= 3) {
                printf("######################################\n\n\n");
                printf("AFS is terminated\n");


                printf("LEFT : FeOre=%d, CoOre=%d, FeIng=%d, CoIng=%d\n",NUM_FE_ORE,NUM_CO_ORE,NUM_FE_ING,NUM_CO_ING);
  
                printf("MADE : CoFe=%d\n",coFeIng);
                thread_exit();
            }
        }

        /*  increate semaphore for working display */
        sema_up(&timer_disp);
    }
}


/*  Function of Robot Arm 1 for a thread  */
void run_RA1() {
    while (true) {
        sema_down(&timer_RA1);
        
        if(sema_try_down(&RA1)) {
            if (railStatus[4] == 0)  {
                railStatus[2] = 0; 
                railStatus[4] = 1;
                sema_up(&IF1);
            } else {
                sema_up(&RA1);
            }
        }
    }
}

/*  Function of Robot Arm 2 for a thread */
void run_RA2() {
    while (true) {
        sema_down(&timer_RA2);
        if (sema_try_down(&RA2)) {
            if (railStatus[STAT_RAIL2OFFSET+4] == 0) {
                railStatus[STAT_RAIL2OFFSET+2] = 0;
                railStatus[STAT_RAIL2OFFSET+4] = 1;
                sema_up(&IF2);
            } else {
                sema_up(&RA2);
            }
        }
    }
}


/*  Function of IF 1 for a thread */
void run_IF1() {
    while(true)  {
        sema_down(&timer_IF1);
        if (sema_try_down(&IF1)) {
            if (railStatus[4] == 1) {
                NUM_FE_ORE--;
                railStatus[4] = 2;
                sema_up(&RA3);
            }
        }   
        sema_up(&timer_RA1);
    }
}

/*  Function of IF 2 for a thread */
void run_IF2() {
    while(true) {
        sema_down(&timer_IF2);
        if (railStatus[STAT_RAIL2OFFSET+4] == 1) {
            railStatus[STAT_RAIL2OFFSET+4] = 2;
            NUM_CO_ORE--;
        }
        if(sema_try_down(&IF2)) {
                sema_up(&RA4);
        }
        sema_up(&timer_RA2);
    }
}


/* Function of Robot Arm 3 for a thread */
void run_RA3() {
    while(true) {
        sema_down(&timer_RA3);
        if (railStatus[6] != 1) {
            if(sema_try_down(&RA3)) {
                if(railStatus[4] == 2) {
                    railStatus[4] = -1;
                    railStatus[6] = -1;
                }
            }
        }
        sema_up(&timer_IF1);
    }
}

/* Function of Robot Arm 4 for a thread */
void run_RA4() {
    while(true) {
        sema_down(&timer_RA4);
        if (railStatus[STAT_RAIL2OFFSET+6] != 1) {
            if(sema_try_down(&RA4)) {
                if(railStatus[STAT_RAIL2OFFSET+4] == 2) {
                    railStatus[STAT_RAIL2OFFSET+4] = -1;
                    railStatus[STAT_RAIL2OFFSET+6] = -1;
                }
            }
        }
        sema_up(&timer_IF2);
    }
}

/*  Function of Robot Arm 5 for a thread */
void run_RA5() {
    while(true) {
        sema_down(&timer_RA5);
        if(sema_try_down(&RA5)) {
            if (railStatus[8] == 1) {
                sema_down(&IF3_feIngIsEmpty);
                railStatus[8] = 0;
                sema_up(&IF3_feIng);
                // pot3 increase
            }
        } 
        sema_up(&timer_RA3);
    }
}

/*  Function of Robot Arm 6 for a thread */
void run_RA6() {
    while(true) {
         
        sema_down(&timer_RA6);
        if(sema_try_down(&RA6)) {
            if(railStatus[STAT_RAIL2OFFSET+8] == 1) {
                sema_down(&IF3_coIngIsEmpty);
                railStatus[STAT_RAIL2OFFSET+8] = 0;
                sema_up(&IF3_coIng);
            }
        }
        sema_up(&timer_RA4);
    }
}


/*  Function for producing time semaphore */
void timeTicker() {
   while (true) {
       timer_sleep(100);
       sema_up(&timeTick);
       timeTickee(); 
   } 
}


/*  every 1 second, semaphore time semaphore produces, and sema up timer_conveyer  */
void timeTickee() {
    sema_down(&timeTick); 
    sema_up(&timer_conveyer);     
}

/*  Function for IF 3 for a thread */
void run_IF3() {
   while (true) {
       sema_down(&IF3_IsAvail);
       railStatus[STAT_POT3OFFSET] = 1; 
       timer_sleep(300);

       for (int i =0; i < NUM_FE_NEED; i++) {
           sema_up(&IF3_feIngIsEmpty);
       }
       for (int i = 0; i < NUM_CO_NEED; i++ ) {
           sema_up(&IF3_coIngIsEmpty);
       }
       railStatus[STAT_POT3OFFSET] = 0;
       coFeIng++;
   }
}


/*  Function for IF 3, input co & fe ings and signaling to produce cofeing  */
void run_IF3_check() {
    while (true) {
        for (int i =0; i < NUM_FE_NEED; i++) {
            sema_down(&IF3_feIng);
        }
        for (int i = 0; i< NUM_CO_NEED; i++) {
            sema_down(&IF3_coIng);
        }
        sema_up(&IF3_IsAvail); 
    }
}



/*  main Function, set arguments ( number of co/fe ores, required numbers of ings  for coFeIng 
 *  this function initialize semaphores, and create threads
 *  */
void run_factorii(char **argv) {
    printf("input : ");
    for (int i = 1; i <= 4; i++) {
        printf("%s  ", argv[i]);
    }

    NUM_FE_NEED = atoi(argv[3]);
    NUM_CO_NEED = atoi(argv[4]);
    printf("%d\n",NUM_CO_NEED);
    NUM_FE_ORE = NUM_FE_ORE_STORED = atoi(argv[1]);
    NUM_CO_ORE = NUM_CO_ORE_STORED = atoi(argv[2]);



    railStatus = (int*)malloc(sizeof(int)*NUM_STAT);
    struct condition testCond;


    sema_init(&timeTick,0);
    /*  initializing semaphores for  Robot Arms  */
    sema_init(&RA1,0); sema_init(&RA3,0); sema_init(&RA5,0);
    sema_init(&RA2,0); sema_init(&RA4,0); sema_init(&RA6,0);
   
   /*  initializing semaphores for IFs */ 
    sema_init(&IF1,0); sema_init(&IF2,0); 

    /* IF3 semas  */ 
    sema_init(&IF3_coIngIsEmpty,NUM_CO_NEED); sema_init(&IF3_feIngIsEmpty,NUM_FE_NEED); sema_init(&IF3_IsAvail,0);
    sema_init(&IF3_coIng,0); sema_init(&IF3_feIng,0);

    /*  timer semaphores initializing */
    sema_init(&timer_RA1,0); sema_init(&timer_IF1,0); sema_init(&timer_RA3,0); sema_init(&timer_RA5,0); 
    sema_init(&timer_RA2,0); sema_init(&timer_IF2,0); sema_init(&timer_RA4,0); sema_init(&timer_RA6,0);
    sema_init(&timer_conveyer,0); sema_init(&timer_disp,1);     


    thread_create("timeTicker", 1, &timeTicker, NULL);
    thread_create("conveyor",1, &run_conveyer, NULL);
    thread_create("RA1",1, &run_RA1, NULL);
    thread_create("RA3",1, &run_RA3, NULL);
    thread_create("RA5",1, &run_RA5, NULL);
    thread_create("IF1",1, &run_IF1, NULL);
    thread_create("IF3",1, &run_IF3, NULL);
    thread_create("IFcheck",1,&run_IF3_check,NULL);
    thread_create("RA2",1, &run_RA2, NULL);
    thread_create("RA4",1, &run_RA4, NULL);
    thread_create("RA6",1, &run_RA6, NULL);
    thread_create("IF2",1, &run_IF2, NULL);
    
   /*  initalize rail Status */ 
    rail_run(railStatus);
    thread_create("display",1,&display_run,railStatus); 
    
}
