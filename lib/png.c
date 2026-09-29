#include <lib/png.h>

#include <stdio.h>
#include <png.h>

// Save bitmap to file
void save_png(struct bitmap_t* bitmap, char* fname)
{
  FILE *fp;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  uint8_t pixel_size = 3;
  uint8_t pixel_depth = 8;
  png_byte ** row_pointers = NULL;
  size_t x, y;
  fp = fopen(fname, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    return;
  }
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    fprintf(stderr, "Error creating write structure\n");
    fclose(fp);
    return;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fprintf(stderr, "Error creating info structure\n");
    fclose(fp);
    return;
  }
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "Error setting jmp\n");
    fclose(fp);
    return;
  }
  png_set_IHDR(png_ptr, info_ptr,
	       bitmap->width, bitmap->height,
	       pixel_depth,
	       PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
	       PNG_FILTER_TYPE_DEFAULT);

  row_pointers = png_malloc(png_ptr, bitmap->height*sizeof(png_byte*));
  for (y = 0; y < bitmap->height; y++) {
    png_byte *row = png_malloc(png_ptr, sizeof(uint8_t)*bitmap->width*pixel_size);
    row_pointers[y] = row;
    for (x = 0; x < bitmap->width; x++) {
      *row++ = bitmap->image_buffer[x + y*bitmap->width].red;
      *row++ = bitmap->image_buffer[x + y*bitmap->width].green;
      *row++ = bitmap->image_buffer[x + y*bitmap->width].blue;
    }
  }
  png_init_io(png_ptr, fp);
  png_set_rows(png_ptr, info_ptr, row_pointers);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  for (y = 0; y < bitmap->height; y++) {
    png_free(png_ptr, row_pointers[y]);
  }
  png_free(png_ptr, row_pointers);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);
}

