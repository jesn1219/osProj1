#ifndef __PROJECTS_FACTORII_FACTORII_H__
#define __PROJECTS_FACTORII_FACTORII_H__

#include "threads/synch.h"



int* railStatus;
struct semaphore timeTick;
struct semaphore timer_disp;
struct semaphore RA1, RA3, RA5, IF1, RA2, RA4, RA6, IF2;
struct semaphore timer_RA1, timer_RA3, timer_RA5, timer_IF1, timer_conveyer;
struct semaphore timer_RA2, timer_RA4, timer_RA6, timer_IF2;
struct semaphore IF3_IsAvail;
struct semaphore IF3_feIng;
struct semaphore IF3_coIng;
struct semaphore IF3_feIngIsEmpty;
struct semaphore IF3_coIngIsEmpty;
struct semaphore TERMINATE;

int NUM_FE_NEED;
int NUM_CO_NEED;
int NUM_FE_ORE_STORED;
int NUM_CO_ORE_STORED;

int NUM_FE_ORE;
int NUM_CO_ORE;

int NUM_FE_ING;
int NUM_CO_ING;

int terminateFlag;
int terminateCount;
int coFeIng;

void run_conveyer();
void run_RA1();
void run_RA2();
void run_IF1();
void run_IF2();
void run_RA3();
void run_RA4();
void run_RA5();
void run_RA6();
void timeTicker();
void timeTickee();
void run_IF3();
void run_IF3_check();
void run_factorii(char **argv);

#endif 
