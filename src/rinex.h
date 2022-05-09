#ifndef RINEX_H
#define RINEX_H


#include "rtklib.h"
#include "satellite.h"
#include "clock.h"

/* constants/macros ----------------------------------------------------------*/

#define SQR(x)      ((x)*(x))

#define NAVEXP      "D"                 /* exponent letter in RINEX NAV */
#define NUMSYS      7                   /* number of systems */
#define MAXRNXLEN   (16*MAXOBSTYPE+4)   /* max RINEX record length */
#define MAXPOSHEAD  1024                /* max head line position */
#define MINFREQ_GLO -7                  /* min frequency number GLONASS */
#define MAXFREQ_GLO 13                  /* max frequency number GLONASS */
#define NINCOBS     262144              /* incremental number of obs data */

#define MAXOBSTYPE  64                  /* max number of obs type in RINEX */


/* type definition -----------------------------------------------------------*/
typedef struct {                        /* signal index type */
    int n;                              /* number of index */
    int idx[MAXOBSTYPE];                /* signal freq-index */
    int pos[MAXOBSTYPE];                /* signal index in obs data (-1:no) */
    uint8_t pri [MAXOBSTYPE];           /* signal priority (15-0) */
    uint8_t type[MAXOBSTYPE];           /* type (0:C,1:L,2:D,3:S) */
    uint8_t code[MAXOBSTYPE];           /* obs-code (CODE_L??) */
    double shift[MAXOBSTYPE];           /* phase shift (cycle) */
} sigind_t;


#endif
