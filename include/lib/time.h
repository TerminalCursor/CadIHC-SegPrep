#ifndef INC_LIB_TIME_H
#define INC_LIB_TIME_H

#include <time.h>

#define TIME(var) struct timespec var;get_time(&var)

// Difference in Time
// Compute the difference between timespec structs
double diff_time(const struct timespec *time1, const struct timespec *time0);

// Get Current Time
void get_time(struct timespec *ts);

#endif
