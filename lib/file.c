#include <lib/file.h>
#include <stdio.h>

// Write array to a file
bool_t write_array(char* output_file_name, void* array, size_t size)
{
  FILE *file = fopen(output_file_name, "wb");
  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", output_file_name);
    return FALSE;
  }
  fwrite(array, size, 1, file);
  fclose(file);
  return TRUE;
}
