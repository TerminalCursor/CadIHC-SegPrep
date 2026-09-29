#ifndef INC_LIB_FILE_H
#define INC_LIB_FILE_H

#include <lib/bool.h>
#include <stddef.h>

// Write array to a file
bool_t write_array(char* output_file_name, void* array, size_t size);

#endif
