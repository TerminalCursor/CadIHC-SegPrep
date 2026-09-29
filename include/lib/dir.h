#ifndef INC_LIB_DIR_H
#define INC_LIB_DIR_H

#include <lib/lib.h>
#include <lib/bool.h>

// Is directory
bool_t is_directory(char* dirname);

// List directory
char** list_directory(char* dirname);

// Get full path
char* full_path(char* dir, char* file);

// Determines if file name has tif file extension
bool_t is_tif_ext(char* file_name);

#endif
