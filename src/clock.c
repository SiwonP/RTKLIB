#include "clock.h"

/* compare precise clock -----------------------------------------------------*/
int CLOCK_comparePreciseClocks(const void *p1, const void *p2)
{
    PreciseClock_t *q1=(PreciseClock_t *)p1,*q2=(PreciseClock_t *)p2;
    // double tt=timediff(q1->time,q2->time);
    double tt=difftime(q1->time.time,q2->time.time)+q1->time.sec-q2->time.sec;
    return tt<-1E-9?-1:(tt>1E-9?1:q1->index-q2->index);
}