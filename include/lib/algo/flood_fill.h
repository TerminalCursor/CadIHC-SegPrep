#ifndef INC_LIB_ALGO_FLOOD_FILL_H
#define INC_LIB_ALGO_FLOOD_FILL_H

#include <lib/bool.h>
#include <lib/color.h>
#include <stddef.h>
#include <stdint.h>

// Flood
// Floods a mask from a given set of image to determine the contiguous regions
//  1. Check that the (x,y) is within the picture
//  2. Check if the (x,y) coordinate in the mask is unused
//  3. Check if the (x,y) coordinate in the image is non-background
//  4. Check if the (x,y) coordinate in the image is the same color as the fill color
//  5. If all hold, set the label for the pixel, and check each neighbor
//     Otherwise, stop flooding
bool_t flood(uint8_t* image, uint16_t* mask, size_t width, size_t height, size_t channels, size_t x, size_t y, uint8_t* fill_color, uint16_t label);

#endif
