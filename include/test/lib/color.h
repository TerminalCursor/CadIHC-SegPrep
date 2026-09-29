#ifndef INC_TEST_LIB_COLOR_H
#define INC_TEST_LIB_COLOR_H

#include <test/__meta__.h>
#ifndef TEST_RESULT
#define TEST_RESULT(subtest,result,n,n_success) _TEST_RESULT("LIB/COLOR",subtest,result,n,n_success)
#endif

#include <lib/color.h>

bool_t test_color_zero(const uint8_t* color1, size_t channels, bool_t result);
bool_t test_color_equal(const uint8_t* color1, const uint8_t* color2, size_t channels, bool_t result);

// Meta test function
bool_t TEST_lib_color();

#endif
