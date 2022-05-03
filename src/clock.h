#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

#include "satellite.h"

/* type definitions ----------------------------------------------------------*/
/**
 * @struct
 * @brief 
 * 
 */
typedef struct GTime {        /* time struct */
    time_t time;        /* time (s) expressed by standard time_t */
    double sec;         /* fraction of second under 1 s */
} gtime_t;


/**
 * @struct PreciseClock_t
 * @brief 
 * 
 */
typedef struct PreciseClock {        /* precise clock type */
    gtime_t time;       /* time (GPST) */
    int index;          /* clock index for multiple files */
    double clk[MAXSAT][1]; /* satellite clock (s) */
    float  std[MAXSAT][1]; /* satellite clock std (s) */
} PreciseClock_t;

int CLOCK_comparePreciseClocks(const void *p1, const void *p2);

#endif