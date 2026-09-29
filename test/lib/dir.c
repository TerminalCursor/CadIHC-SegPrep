#include <test/lib/dir.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool_t test_is_directory(char* dirname, bool_t result)
{
  bool_t fcall_result = is_directory(dirname);
  if (fcall_result == result) {
    return TRUE;
  }
  return FALSE;
}

void  _TEST_is_directory(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: sample_data/ (TRUE) [On clean git folder]
  sub_result = test_is_directory("sample_data", TRUE);
  *result &= sub_result;
  TEST_RESULT("IS_DIRECTORY",sub_result, *test_count, (*test_pass));

  // Test 2: asdf/ (FALSE) [On clean git folder]
  sub_result = test_is_directory("asdf", FALSE);
  *result &= sub_result;
  TEST_RESULT("IS_DIRECTORY",sub_result, *test_count, (*test_pass));
}

bool_t test_full_path(char* dirname, char* file, char* result)
{
  char* fpath = full_path(dirname, file);
  bool_t cmp_result = strcmp(result, fpath);
  free(fpath);
  if (cmp_result == 0) {
    return TRUE;
  }
  return FALSE;
}

void  _TEST_full_path(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: sample_data/small + small.tif = sample_data/small/small.tif
  sub_result = test_full_path("sample_data/small", "small.tif", "sample_data/small/small.tif");
  *result &= sub_result;
  TEST_RESULT("FULL_PATH",sub_result, *test_count, (*test_pass));
}

bool_t test_is_tif_ext(char* file_name, bool_t result)
{
  size_t file_name_len = strlen(file_name);
  file_name_len -= 3;
  bool_t cmp_result = strcmp(file_name+file_name_len, "tif");
  if (cmp_result == 0) {
    return TRUE == result;
  }
  return FALSE == result;
}

void  _TEST_is_tif_ext(bool_t* result, uint16_t* test_count, uint16_t* test_pass)
{
  bool_t sub_result;
  // Test 1: sample_data/small/small.tif (TRUE)
  sub_result = test_is_tif_ext("sample_data/small/small.tif", TRUE);
  *result &= sub_result;
  TEST_RESULT("IS_TIF_EXT",sub_result, *test_count, (*test_pass));

  // Test 2: data/test.tif (TRUE)
  sub_result = test_is_tif_ext("data/test.tif", TRUE);
  *result &= sub_result;
  TEST_RESULT("IS_TIF_EXT",sub_result, *test_count, (*test_pass));

  // Test 3: sample_data/small/small (FALSE)
  sub_result = test_is_tif_ext("sample_data/small/small", FALSE);
  *result &= sub_result;
  TEST_RESULT("IS_TIF_EXT",sub_result, *test_count, (*test_pass));
}

bool_t TEST_lib_dir()
{
  uint16_t test_count = 0;
  uint16_t test_pass = 0;
  bool_t result = TRUE;

  // Testing directory existence
  _TEST_is_directory(&result, &test_count, &test_pass);

  // Testing full path
  _TEST_full_path(&result, &test_count, &test_pass);

  // Testing full path
  _TEST_is_tif_ext(&result, &test_count, &test_pass);

  return test_count == test_pass;
}
