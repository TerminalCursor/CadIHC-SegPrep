#include <lib/algo/flood_fill.h>

// Flood
// Floods a mask from a given set of image to determine the contiguous regions
//  1. Check that the (x,y) is within the picture
//  2. Check if the (x,y) coordinate in the mask is unused
//  3. Check if the (x,y) coordinate in the image is non-background
//  4. Check if the (x,y) coordinate in the image is the same color as the fill color
//  5. If all hold, set the label for the pixel, and check each neighbor
//     Otherwise, stop flooding
bool_t flood(uint8_t* image, uint16_t* mask, size_t width, size_t height, size_t channels, size_t x, size_t y, uint8_t* fill_color, uint16_t label)
{
  if ((x >= width) | (y >= height)) {
    return FALSE;
  }
  size_t coord = x + y*width;
  if (mask[coord] != 0) {
    return FALSE;
  }
  if (color_zero(&(image[coord*channels]), channels)) {
    return FALSE;
  }
  if (color_equal(&(image[coord*channels]), fill_color, channels)) {
    mask[coord] = label;
    flood(image, mask, width, height, channels, x, y+1, fill_color, label);
    flood(image, mask, width, height, channels, x, y-1, fill_color, label);
    flood(image, mask, width, height, channels, x+1, y, fill_color, label);
    flood(image, mask, width, height, channels, x-1, y, fill_color, label);
    return TRUE;
  }
  return FALSE;
}

