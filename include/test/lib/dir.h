#ifndef INC_TEST_LIB_DIR_H
#define INC_TEST_LIB_DIR_H

#include <test/__meta__.h>
#ifndef TEST_RESULT
#define TEST_RESULT(subtest,result,n,n_success) _TEST_RESULT("LIB/DIR",subtest,result,n,n_success)
#endif

#include <lib/dir.h>

bool_t test_is_directory(char* dirname, bool_t result);
bool_t test_full_path(char* dirname, char* file, char* result);
bool_t test_is_tif_ext(char* file_name, bool_t result);

// Meta test function
bool_t TEST_lib_dir();

#endif
