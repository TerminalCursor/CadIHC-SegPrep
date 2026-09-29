#ifndef INC_LIB_PNG_H
#define INC_LIB_PNG_H

#include <stdint.h>
#include <sys/types.h>

struct pixel_t {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct bitmap_t {
  struct pixel_t* image_buffer;
  size_t width;
  size_t height;
};

// Save bitmap to file
void save_png(struct bitmap_t* bitmap, char* fname);

#endif
