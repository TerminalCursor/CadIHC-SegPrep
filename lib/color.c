#include <stdio.h>
#include <lib/color.h>
#include <lib/bool.h>

// Color Equal to Background
// Background: zeros in RGB, alpha can be whatever
bool_t color_zero(const uint8_t* color1, size_t channels) {
  for (size_t idx = 0; idx < channels; idx++) {
    if (idx == 3) {
      break;
    }
    if (color1[idx] != 0) {
      return FALSE;
    }
  }
  return TRUE;
}

// Color Equality
// Determine if two colors are identical
bool_t color_equal(const uint8_t* color1, const uint8_t* color2, size_t channels) {
  for (size_t idx = 0; idx < channels; idx++) {
    if (color1[idx] != color2[idx]) {
      return FALSE;
    }
  }
  return TRUE;
}

// Print out the `channels` length color vector
void print_color(uint8_t* color, size_t channels) {
  for (size_t index = 0; index < channels; index++) {
    printf("%hhu ", color[index]);
  }
  printf("\n");
}
