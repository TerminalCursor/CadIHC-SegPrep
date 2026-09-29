 #include <lib/seg/util.h>
#include <lib/algo/flood_fill.h>
#include <lib/png.h>
#include <tiffio.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Suppress Tiff Warnings
void TiffDummyHandler(const char* module, const char* fmt, va_list ap)
{
    // ignore errors and warnings (or handle them your own way)
}

// Convert x,y coords to linear coordinate
size_t xy_to_coord(size_t x, size_t y, uint32_t width, uint32_t height)
{
  return x + y*width;
}

// Determine if coordinate is on a mask boundary
//  Assumes mask is (WxH)
bool_t is_on_mask_boundary(uint16_t* mask, uint32_t width, uint32_t height, size_t x, size_t y)
{
  size_t starting_coord = xy_to_coord(x, y, width, height);
  size_t proposed_position;
  uint16_t current_value = mask[starting_coord];

  // Left neighbor
  if (x != 0) {
    proposed_position = xy_to_coord(x-1, y, width, height);
    if (mask[proposed_position] != current_value) {
      return TRUE;
    }
  }
  // Right neighbor
  if ((x+1) != width) {
    proposed_position = xy_to_coord(x+1, y, width, height);
    if (mask[proposed_position] != current_value) {
      return TRUE;
    }
  }
  if (y != 0) {
    proposed_position = xy_to_coord(x, y-1, width, height);
    if (mask[proposed_position] != current_value) {
      return TRUE;
    }
  }
  if ((y+1) != height) {
    proposed_position = xy_to_coord(x, y+1, width, height);
    if (mask[proposed_position] != current_value) {
      return TRUE;
    }
  }
  return FALSE;
}

// Dilate masks by one 4-connected pixel
uint16_t* _dilate(uint16_t* mask, uint32_t width, uint32_t height)
{
  uint16_t *new_mask = (uint16_t*)calloc(width*height,sizeof(uint16_t));
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t current_position = xy_to_coord(x, y, width, height);
      if (mask[current_position] != 0) {
	new_mask[current_position] = mask[current_position];
	continue;
      }
      size_t proposed_position;
      if (x != 0) {
	proposed_position = xy_to_coord(x-1, y, width, height);
	if (mask[proposed_position] != 0) {
	  new_mask[current_position] = mask[proposed_position];
	  continue;
	}
      }
      if ((x+1) != width) {
	proposed_position = xy_to_coord(x+1, y, width, height);
	if (mask[proposed_position] != 0) {
	  new_mask[current_position] = mask[proposed_position];
	  continue;
	}
      }
      if (y != 0) {
	proposed_position = xy_to_coord(x, y-1, width, height);
	if (mask[proposed_position] != 0) {
	  new_mask[current_position] = mask[proposed_position];
	  continue;
	}
      }
      if ((y+1) != height) {
	proposed_position = xy_to_coord(x, y+1, width, height);
	if (mask[proposed_position] != 0) {
	  new_mask[current_position] = mask[proposed_position];
	  continue;
	}
      }
    }
  }
  return new_mask;
}

// Dilate masks by one 4-connected pixel
void dilate(uint16_t** mask, uint32_t width, uint32_t height)
{
  uint16_t *new_mask = _dilate(*mask, width, height);
  if (new_mask != NULL) {
    free(*mask);
    *mask = new_mask;
  }
}

// Erode masks by one 4-connected pixel
uint16_t* _erode(uint16_t* mask, uint32_t width, uint32_t height)
{
  uint16_t *new_mask = (uint16_t*)calloc(width*height,sizeof(uint16_t));
  memcpy(new_mask, mask, width*height*sizeof(uint16_t));
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t current_position = xy_to_coord(x, y, width, height);
      size_t proposed_position;
      if (x != 0) {
	proposed_position = xy_to_coord(x-1, y, width, height);
	if (mask[proposed_position] == 0) {
	  new_mask[current_position] = 0;
	  continue;
	}
      }
      if ((x+1) != width) {
	proposed_position = xy_to_coord(x+1, y, width, height);
	if (mask[proposed_position] == 0) {
	  new_mask[current_position] = 0;
	  continue;
	}
      }
      if (y != 0) {
	proposed_position = xy_to_coord(x, y-1, width, height);
	if (mask[proposed_position] == 0) {
	  new_mask[current_position] = 0;
	  continue;
	}
      }
      if ((y+1) != height) {
	proposed_position = xy_to_coord(x, y+1, width, height);
	if (mask[proposed_position] == 0) {
	  new_mask[current_position] = 0;
	  continue;
	}
      }
    }
  }
  return new_mask;
}

// Erode masks by one 4-connected pixel
void erode(uint16_t** mask, uint32_t width, uint32_t height)
{
  uint16_t *new_mask = _erode(*mask, width, height);
  if (new_mask != NULL) {
    free(*mask);
    *mask = new_mask;
  }
}

// Close up masks by N-pixels
uint16_t* _closeup(uint16_t* mask, uint32_t width, uint32_t height, size_t num_pixels)
{
  uint16_t *new_mask = (uint16_t*)calloc(width*height,sizeof(uint16_t));
  memcpy(new_mask, mask, width*height*sizeof(uint16_t));
  for (size_t count = 0; count < num_pixels; count++) {
    dilate(&new_mask, width, height);
  }
  for (size_t count = 0; count < num_pixels; count++) {
    erode(&new_mask, width, height);
  }
  // Retain original mask at the very least
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      if (mask[coord] != 0) {
	if (new_mask[coord] != mask[coord]) {
	  new_mask[coord] = mask[coord];
	}
      }
    }
  }
  return new_mask;
}

// Close up masks by N-pixels
//  Update pointer
void closeup(uint16_t** mask, uint32_t width, uint32_t height, size_t num_pixels)
{
  uint16_t *new_mask = _closeup(*mask, width, height, num_pixels);
  if (new_mask != NULL) {
    free(*mask);
    *mask = new_mask;
  }
}

// Combine Label Masks
// For all empty spaces in the destination, put the extra label if it exists
// Allocates an array if destination is unallocated
uint16_t* combine_masks(uint16_t *destination, uint16_t *extra_labels, uint32_t width, uint32_t height)
{
  if (destination == NULL) {
    destination = (uint16_t*)calloc(width*height, sizeof(uint16_t));
  }
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      if (destination[coord] == 0) {
	destination[coord] = extra_labels[coord];
      }
    }
  }
  return destination;
}

// Process Tif File to Labels
//  width, height will be overwritten with image dimensions
//  starting_label_p will be incremented for each label found in the image
uint16_t* tif_to_labels(char* tif_file_name, uint32_t *width, uint32_t *height, uint16_t *starting_label_p)
{
  TIFFSetWarningHandler(TiffDummyHandler);
  //-TIFF-IMAGE-OPEN-------------------------------
  TIFF *tif = TIFFOpen(tif_file_name, "r");
  if (!tif) {
    fprintf(stderr, "Failed to open TIFF file\n");
    return NULL;
  }

  //-TIFF-FIND-DIMENSIONS--------------------------
  size_t channels = 1;
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, height);

  tmsize_t STRIP_LENGTH = TIFFStripSize(tif);
  tmsize_t STRIP_COUNT = TIFFNumberOfStrips(tif);
  if ((*width)*(*height)*3 == STRIP_LENGTH*STRIP_COUNT) {
    channels = 3;
  } else if ((*width)*(*height)*4 == STRIP_LENGTH*STRIP_COUNT) {
    channels = 4;
  }

  //-TIFF-LOAD-DATA--------------------------------
  void* buffer = malloc(STRIP_LENGTH*sizeof(uint8_t));
  if (buffer == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    TIFFClose(tif);
    return NULL;
  }
  uint8_t* image_data = calloc((*width)*(*height)*channels,sizeof(uint8_t));
  if (image_data == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    free(buffer);
    TIFFClose(tif);
    return NULL;
  }
  for (size_t y = 0; y < STRIP_COUNT; y++) {
    tmsize_t strip_size = TIFFReadRawStrip(tif, y, buffer, STRIP_LENGTH);
    assert(strip_size == STRIP_LENGTH);
    for (size_t x = 0; x < STRIP_LENGTH; x++) {
      image_data[x + y*STRIP_LENGTH] = ((uint8_t*)buffer)[x];
    }
  }
  free(buffer);

  //-FLOOD-FILL-SEGMENTATION-----------------------
  //-CONTIGUOUS-REGION-FINDING---------------------
  uint16_t *labels = NULL;
  labels = (uint16_t*)calloc((*width)*(*height),sizeof(uint16_t));
  if (labels == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    free(image_data);
    TIFFClose(tif);
    return NULL;
  }
  // Flood fill on each pixel
  //  Increase label for each success
  for (size_t y = 0; y < *height; y++) {
    for (size_t x = 0; x < *width; x++) {
      size_t coord = x + y*(*width);
      if(flood(image_data, labels, *width, *height, channels, x, y, &(image_data[coord*channels]), *starting_label_p)) {
        *starting_label_p += 1;
      }
    }
  }
  free(image_data);
  TIFFClose(tif);
  return labels;
}

// Convert mask to bitmap
struct bitmap_t* uint16_to_bitmap(uint16_t* buffer, uint32_t width, uint32_t height)
{
  struct pixel_t* out_buffer = (struct pixel_t*)calloc(width*height, sizeof(struct pixel_t));
  if (out_buffer == NULL) {
    return NULL;
  }
  struct bitmap_t* bitmap = (struct bitmap_t*)malloc(sizeof(struct bitmap_t));
  if (bitmap == NULL) {
    free(out_buffer);
    return NULL;
  }
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      uint8_t red   = (buffer[coord] & 0xF00) >> 4*2;
      uint8_t green = (buffer[coord] & 0x0F0) >> 4*1;
      uint8_t blue  = (buffer[coord] & 0x00F) >> 4*0;
      out_buffer[coord].red = red | (red << 4);
      out_buffer[coord].green = green | (green << 4);
      out_buffer[coord].blue = blue | (blue << 4);
    }
  }
  bitmap->image_buffer = out_buffer;
  bitmap->width = (size_t)width;
  bitmap->height = (size_t)height;
  return bitmap;
}

// Reduce a mask to the contiguous regions
uint16_t* _reduce_contiguous_regions(uint16_t* masks, uint32_t width, uint32_t height, uint16_t* total_labels)
{
  uint16_t starting_label = 1;
  uint16_t* new_masks = (uint16_t*)calloc(width*height, sizeof(uint16_t));
  if (new_masks == NULL) {
    return NULL;
  }
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      uint8_t channels = 2; // uint16_t = 2*uint8_t
      if (flood((uint8_t*)masks, new_masks, width, height, channels, x, y, &(((uint8_t*)masks)[coord*channels]), starting_label)) {
	starting_label++;
      }
    }
  }
  if (total_labels != NULL) {
    *total_labels = starting_label;
  }
  return new_masks;
}

// Reduce a mask to the contiguous regions
//  Automatically update pointer to contiguous mask
//  Freeing previous mask
void reduce_contiguous_regions(uint16_t** masks_p, uint32_t width, uint32_t height, uint16_t* total_labels)
{
  if (masks_p == NULL) {
    return;
  }
  uint16_t* new_masks = _reduce_contiguous_regions(*masks_p, width, height, total_labels);
  if (new_masks != NULL) {
    free(*masks_p);
    *masks_p = new_masks;
  }
}
