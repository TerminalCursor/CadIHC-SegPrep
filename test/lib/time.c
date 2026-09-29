#include <test/lib/time.h>

const struct timespec zero = {0,0};
const struct timespec one_s = {1,0};
const struct timespec one_ns = {0,1};
const struct timespec one_s_ns = {1,1};

bool_t test_diff_time(const struct timespec *time1, const struct timespec *time0, double result)
{
  double fcall_result = diff_time(time1, time0);
  if (fcall_result == result) {
    return TRUE;
  }
  return FALSE;
}

void _TEST_diff_time(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: 0-0=0
  sub_result = test_diff_time(&zero, &zero, 0.0);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));

  // Test 2: 1s-0=1s
  sub_result = test_diff_time(&one_s, &zero, 1.0);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));

  // Test 3: 1ns-0=1ns
  sub_result = test_diff_time(&one_ns, &zero, 0.000000001);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));

  // Test 4: 1s1ns-0=1s1ns
  sub_result = test_diff_time(&one_s_ns, &zero, 1.000000001);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));

  // Test 5: 1s1ns-1ns=1s
  sub_result = test_diff_time(&one_s_ns, &one_ns, 1.0);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));

  // Test 6: 1s1ns-1s=1ns
  sub_result = test_diff_time(&one_s_ns, &one_s, 0.000000001);
  *result &= sub_result;
  TEST_RESULT("DIFF_TIME",sub_result, *test_count, (*test_pass));
}

bool_t TEST_lib_time()
{
  uint16_t test_count = 0;
  uint16_t test_pass = 0;
  bool_t result = TRUE;

  // Testing directory existence
  _TEST_diff_time(&result, &test_count, &test_pass);

  return test_count == test_pass;
}
