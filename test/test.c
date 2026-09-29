#include <test/lib/color.h>
#include <test/lib/dir.h>
#include <test/lib/time.h>

#define _META_TEST_RESULT(name,result) if (result) { fprintf(stderr, "       \x1b[92mPASS\x1b[0m %s\n", name);} else { fprintf(stderr, "%s: \x1b[91mFAIL\x1b[0m\n", name);}

int main()
{
  bool_t all_success = TRUE;
  bool_t test_success;
  // lib/color.c Test
  test_success = TEST_lib_color();
  all_success &= test_success;
  _META_TEST_RESULT("LIB/COLOR", test_success)

  // lib/dir.c Test
  test_success = TEST_lib_dir();
  all_success &= test_success;
  _META_TEST_RESULT("LIB/DIR", test_success)

  // lib/time.c Test
  test_success = TEST_lib_time();
  all_success &= test_success;
  _META_TEST_RESULT("LIB/TIME", test_success)

  if (all_success == TRUE) {
    return 0;
  }
  return 1;
}
