#include <lib/dir.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

// Is directory
bool_t is_directory(char* dirname) {
  struct stat st;
  if (stat(dirname, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      return TRUE;
    }
  }
  return FALSE;
}

// List directory
char** list_directory(char* dirname) {
  DIR *d;
  struct dirent *dir;
  d = opendir(dirname);
  char** file_names = (char**)malloc(sizeof(char*));
  if (!file_names) {
    return NULL;
  }
  file_names[0] = NULL;
  size_t file_count = 0;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
	// When a regular file is reached
	/// Create space for it in the list
	file_names = realloc(file_names, (file_count+1+1)*sizeof(char*));
	/// Create space for the name
	file_names[file_count] = calloc(strlen(dir->d_name)+1, sizeof(char));
	/// Copy the name
	strcpy(file_names[file_count], dir->d_name);
	/// Mark the end of the file list with a NULL entry
	file_names[++file_count] = NULL;
      }
    }
    return file_names;
  }
  return NULL;
}

// Get full path
char* full_path(char* dir, char* file)
{
  char* fpath = NULL;
  size_t dir_len = strlen(dir);
  size_t file_len = strlen(file);
  fpath = (char*)calloc(dir_len+file_len+2,sizeof(char));
  strcpy(fpath,dir);
  strcpy(fpath+dir_len+1,file);
  fpath[dir_len] = '/';
  return fpath;
}

// Determines if file name has tif file extension
bool_t is_tif_ext(char* file_name)
{
  size_t file_len = strlen(file_name);
  if ((file_name[file_len-3] == 't') && (file_name[file_len-2] == 'i') && (file_name[file_len-1] == 'f')) {
    return TRUE;
  }
  return FALSE;
}
