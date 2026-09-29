#ifndef INC_LIB_SEG_UTIL_H
#define INC_LIB_SEG_UTIL_H

#include <lib/bool.h>
#include <stdint.h>
#include <stddef.h>

// Convert x,y coords to linear coordinate
size_t xy_to_coord(size_t x, size_t y, uint32_t width, uint32_t height);

// Determine if coordinate is on a mask boundary
//  Assumes mask is (WxH)
bool_t is_on_mask_boundary(uint16_t* mask, uint32_t width, uint32_t height, size_t x, size_t y);

// Dilate masks by one 4-connected pixel
void dilate(uint16_t** mask, uint32_t width, uint32_t height);

// Erode masks by one 4-connected pixel
void erode(uint16_t** mask, uint32_t width, uint32_t height);

// Close up masks by N-pixels
void closeup(uint16_t** mask, uint32_t width, uint32_t height, size_t count);

// Combine Label Masks
// For all empty spaces in the destination, put the extra label if it exists
// Allocates an array if destination is unallocated
uint16_t* combine_masks(uint16_t *destination, uint16_t *extra_labels, uint32_t width, uint32_t height);

// Process Tif File to Labels
//  width, height will be overwritten with image dimensions
//  starting_label_p will be incremented for each label found in the image
uint16_t* tif_to_labels(char* tif_file_name, uint32_t *width, uint32_t *height, uint16_t *starting_label_p);

// Convert mask to bitmap
struct bitmap_t* uint16_to_bitmap(uint16_t* buffer, uint32_t width, uint32_t height);

// Reduce a mask to the contiguous regions
//  Automatically update pointer to contiguous mask
//  Freeing previous mask
void reduce_contiguous_regions(uint16_t** masks_p, uint32_t width, uint32_t height, uint16_t* total_labels);

#endif
