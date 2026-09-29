#ifndef INC_TEST___META___H
#define INC_TEST___META___H

#include <lib/bool.h>
#include <stdio.h>

#ifdef TEST_SHOW_PASS
#define _TEST_PASS(s,sub,n) fprintf(stderr, "(%4X) \x1b[92mPASS\x1b[0m %s/%s\n", ++n, s, sub)
#else
#define _TEST_PASS(s,sub,n) ++n
#endif

#define _TEST_FAIL(s,sub,n) fprintf(stderr, "(%4X) \x1b[91mFAIL\x1b[0m %s/%s\n", ++n, s, sub)

#ifndef _TEST_RESULT
#define _TEST_RESULT(main_string,subtest_string,result,n,n_success) if (!result) {_TEST_FAIL(main_string,subtest_string, n);} else {_TEST_PASS(main_string,subtest_string,n);n_success++;}
#endif

#endif
