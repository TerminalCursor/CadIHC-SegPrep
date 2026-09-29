#ifndef INC_TEST_LIB_TIME_H
#define INC_TEST_LIB_TIME_H

#include <test/__meta__.h>
#ifndef TEST_RESULT
#define TEST_RESULT(subtest,result,n,n_success) _TEST_RESULT("LIB/TIME",subtest,result,n,n_success)
#endif

#include <lib/time.h>

bool_t test_diff_time(const struct timespec *time1, const struct timespec *time0, double result);

// Meta test function
bool_t TEST_lib_time();

#endif
