#ifndef INC_LIB_COLOR_H
#define INC_LIB_COLOR_H

#include <stdint.h>
#include <stddef.h>
#include <lib/bool.h>

// Color Equal to Background
// Background: zeros in RGB, alpha can be whatever
bool_t color_zero(const uint8_t* color1, size_t channels);

// Color Equality
// Determine if two colors are identical
bool_t color_equal(const uint8_t* color1, const uint8_t* color2, size_t channels);

// Print out the `channels` length color vector
void print_color(uint8_t* color, size_t channels);

#endif
