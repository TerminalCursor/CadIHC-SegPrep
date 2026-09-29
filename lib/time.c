#include <lib/time.h>

// Difference in Time
// Compute the difference between timespec structs
double diff_time(const struct timespec *time1, const struct timespec *time0) {
  return (time1->tv_sec - time0->tv_sec)
      + (time1->tv_nsec - time0->tv_nsec) / 1000000000.0;
}

// Get Current Time
void get_time(struct timespec *ts) {
  timespec_get(ts, TIME_UTC);
}
