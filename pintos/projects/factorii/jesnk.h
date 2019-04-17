/* Project name : AFS system
 * file name : jesnk.h
 * last modified date : 2019.04.17
 * description : 
 * this file define many values for AFS system
 *
 */
#ifndef JESNK_H
#define JESNK_H


/*  Basic Vaules */
#define RAIL_COL 10
#define RAIL_ROW 5
#define RAIL1OFFSET RAIL_COL
#define RAIL2OFFSET RAIL_COL*3
#define NUM_STAT RAIL_COL*2+1

/*  OFFSETS for display array */
#define DIS_POT_1_OFFSET 4
#define DIS_RBA_1_OFFSET 3
#define DIS_RBA_2_OFFSET 5
#define DIS_RBA_3_OFFSET 9
#define DIS_POT_2_OFFSET 9
#define DISP_RAIL2OFFSET RAIL_COL



/*  OFFSETS for rail status array */
#define STAT_RAIL1OFFSET 0
#define STAT_RAIL2OFFSET RAIL_COL

#define STAT_POT1OFFSET (4)
#define STAT_POT2OFFSET (RAIL_COL+4)
#define STAT_POT3OFFSET (RAIL_COL*2) 
#define STAT_RBA1_1OFFSET 3 
#define STAT_RBA1_2OFFSET 5 
#define STAT_RBA1_3OFFSET 9
#define STAT_RBA2_1OFFSET RAIL_COL +  3 
#define STAT_RBA2_2OFFSET RAIL_COL + 5 
#define STAT_RBA2_3OFFSET RAIL_COL + 9 

/*  Symbols for display */
#define SYBL_DIV 'X'
#define SYBL_RBA '@'


#endif // JESNK_H
