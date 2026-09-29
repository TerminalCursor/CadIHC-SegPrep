#include <test/lib/color.h>

const uint8_t test_rgba[4] = {0,0,0,0};
const uint8_t test_Rgba[4] = {1,0,0,0};
const uint8_t test_rGba[4] = {0,1,0,0};
const uint8_t test_rgBa[4] = {0,0,1,0};
const uint8_t test_rgbA[4] = {0,0,0,1};

bool_t test_color_zero(const uint8_t* color1, size_t channels, bool_t result)
{
  bool_t fcall_result = color_zero(color1, channels);
  if (fcall_result == result) {
    return TRUE;
  }
  return FALSE;
}

void _TEST_color_zero(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: 1 channel zero color
  //  Should result: True
  sub_result = test_color_zero(test_rgba, 1, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 2: 1 channel non-zero color
  //  Should result: False
  sub_result = test_color_zero(test_Rgba, 1, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 3: 2 channel zero color
  //  Should result: True
  sub_result = test_color_zero(test_rgba, 2, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 4: 2 channel non-zero color
  //  Should result: False
  sub_result = test_color_zero(test_rGba, 2, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 5: 3 channel zero color
  //  Should result: True
  sub_result = test_color_zero(test_rgba, 3, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 6: 3 channel non-zero color
  //  Should result: False
  sub_result = test_color_zero(test_rgBa, 3, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 7: 4 channel zero color
  //  Should result: True
  sub_result = test_color_zero(test_rgba, 4, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 8: 4 channel non-zero color
  //  Should result: False
  sub_result = test_color_zero(test_rgBa, 4, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));

  // Test 9: 4 channel non-zero color (Alpha non-zero)
  //  Should result: True
  sub_result = test_color_zero(test_rgbA, 4, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_ZERO",sub_result, *test_count, (*test_pass));
}

bool_t test_color_equal(const uint8_t* color1, const uint8_t* color2, size_t channels, bool_t result)
{
  bool_t fcall_result = color_equal(color1, color2, channels);
  if (fcall_result == result) {
    return TRUE;
  }
  return FALSE;
}

void _TEST_color_equal(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: 1 channel equal (zero)
  //  Should result: True
  sub_result = test_color_equal(test_rgba, test_rgba, 1, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 2: 1 channel equal (nonzero)
  //  Should result: True
  sub_result = test_color_equal(test_Rgba, test_Rgba, 1, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 3: 1 channel nonequal
  //  Should result: True
  sub_result = test_color_equal(test_rgba, test_Rgba, 1, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 4: 2 channel equal (zero)
  //  Should result: True
  sub_result = test_color_equal(test_rgba, test_rgba, 2, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 5: 2 channel equal (nonzero)
  //  Should result: True
  sub_result = test_color_equal(test_rGba, test_rGba, 2, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 6: 2 channel nonequal
  //  Should result: False
  sub_result = test_color_equal(test_Rgba, test_rGba, 2, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 7: 3 channel equal (zero)
  //  Should result: True
  sub_result = test_color_equal(test_rgba, test_rgba, 3, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 8: 3 channel equal (nonzero)
  //  Should result: True
  sub_result = test_color_equal(test_rgBa, test_rgBa, 3, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 9: 3 channel nonequal
  //  Should result: False
  sub_result = test_color_equal(test_Rgba, test_rgBa, 3, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 7: 4 channel equal (zero)
  //  Should result: True
  sub_result = test_color_equal(test_rgba, test_rgba, 4, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 8: 4 channel equal (nonzero)
  //  Should result: True
  sub_result = test_color_equal(test_rgbA, test_rgbA, 4, TRUE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));

  // Test 9: 4 channel nonequal
  //  Should result: False
  sub_result = test_color_equal(test_Rgba, test_rgbA, 4, FALSE);
  *result &= sub_result;
  TEST_RESULT("COLOR_EQUAL",sub_result, *test_count, (*test_pass));
}

// Meta test function
bool_t TEST_lib_color()
{
  uint16_t test_count = 0;
  uint16_t test_pass = 0;
  bool_t result = TRUE;

  // Testing color_zero
  _TEST_color_zero(&result, &test_count, &test_pass);

  // Testing color_equal
  _TEST_color_equal(&result, &test_count, &test_pass);

  return test_count == test_pass;
}
