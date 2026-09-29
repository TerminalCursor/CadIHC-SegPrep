#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lib/lib.h>
#include <lib/png.h>
#include <lib/bool.h>
#include <lib/monad.h>
#include <lib/dir.h>
#include <lib/file.h>
#include <lib/time.h>
#include <lib/color.h>
#include <lib/algo/flood_fill.h>
#include <lib/algo/avl_tree.h>
#include <lib/seg/util.h>
#include <lib/seg/mask_data.h>

int main(int argc, char** argv)
{
  char opt;
  char* directory = NULL;
  char* png_file = "../out.png";
  char* bin_file = "../out.bin";
  size_t closeup_pixel_count = 10;
  size_t min_area = 500;
  size_t min_perimeter = 0;
  bool_t silent = FALSE;
  //-----------------------------------------------
  //-GET-COMMAND-LINE-ARGUMENTS--------------------
  //-----------------------------------------------
  while ((opt = getopt(argc, argv, "d:b:p:n:sA:P:")) != -1) {
    switch (opt) {
    case 's':
      silent = TRUE;
      break;
    case 'A':
      min_area = atoi(optarg);
      break;
    case 'P':
      min_perimeter = atoi(optarg);
      break;
    case 'd':
      if (!silent) {
        printf("Parse Directory: %s\n", optarg);
      }
      directory = optarg;
      break;
    case 'b':
      if (!silent) {
	printf("Bin File: %s\n", optarg);
      }
      bin_file = optarg;
      break;
    case 'p':
      if (!silent) {
	printf("PNG File: %s\n", optarg);
      }
      png_file = optarg;
      break;
    case 'n':
      if (!silent) {
	printf("Closeup Size: %d\n", atoi(optarg));
      }
      closeup_pixel_count = atoi(optarg);
      break;
    case ':':
      if (!silent) {
	printf("Option requires value\n");
      }
      break;
    case '?':
      if (!silent) {
	printf("Unknown option: %c\n", optopt);
      }
      break;
    }
  }
  if (!silent) {
    for (;optind < argc; optind++) {
      printf("Extra arguments: %s\n", argv[optind]);
    }
  }
  TIME(ts_g_start);
  //-----------------------------------------------
  //-PROCESS-FILES-IN-DIRECTORY--------------------
  //-----------------------------------------------
  char** file_list = NULL;
  uint32_t width, height;
  uint16_t starting_label = 1;
  uint16_t *masks = NULL;
  // Expect a directory to be passed as the first argument
  if (directory != NULL) {
    // Ensure the directory exists
    if (is_directory(directory)) {
      // List files in the ddirectory
      file_list = list_directory(directory);
      if (file_list != NULL) {
	for (size_t index = 0; file_list[index] != NULL; index++) {
	  char* fname = file_list[index];
	  if (is_tif_ext(fname) == FALSE) {
	    free(file_list[index]);
	    continue;
	  }
	  // If we have a tiff file
	  //  1. Convert to labels
	  //  2. Find contiguous regions
	  //  3. Combine with current total mask
	  //  4. Free up allocations made in this process
	  char* fpath = full_path(directory, fname);
	  if (fpath == NULL) {
	    free(file_list[index]);
	    continue;
	  }
	  if (!silent) {
	    printf("Loading %s...\n", fpath);
	  }
	  //-----------------------------------------------
	  //-PROCESS-TIFF-TO-LABELS------------------------
	  //-----------------------------------------------
	  uint16_t *file_labels = tif_to_labels(fpath, &width, &height, &starting_label);
	  if (file_labels == NULL) {
	    free(fpath);
	    free(file_list[index]);
	    continue;
	  }
	  //-----------------------------------------------
	  //-COMBINE-LABELS-TO-GLOBAL-MASK-----------------
	  //-----------------------------------------------
	  masks = combine_masks(masks, file_labels, width, height);
	  free(file_labels);
	  free(fpath);
	  free(file_list[index]);
	}
	free(file_list);
      }
    }
  }
  if (masks == NULL) {
    fprintf(stderr, "No masks found!\n");
    return 1;
  }

  //-----------------------------------------------
  //-FIND-CONTIGUOUS-REGIONS-----------------------
  //-----------------------------------------------
  reduce_contiguous_regions(&masks, width, height, &starting_label);
  if (!silent) {
    printf("%u labels found\n", starting_label-1);
    printf("Mask dimensions: %u %u\n", width, height);
  }
  //-----------------------------------------------
  //-FILTER-SMALL-REGIONS-OUT----------------------
  //-----------------------------------------------
  TIME(ts_filter_start);
  filter_small_masks(masks, width, height, min_area, min_perimeter);
  TIME(ts_filter_end);
  if (!silent) {
    printf("Removing small labels took %f ms\n", 1000*diff_time(&ts_filter_end, &ts_filter_start));
  }
  //-----------------------------------------------
  //-FIND-CONTIGUOUS-REGIONS-----------------------
  //-----------------------------------------------
  //--to-make-labels-span-1-to-n-------------------
  //-----------------------------------------------
  reduce_contiguous_regions(&masks, width, height, &starting_label);
  if (!silent) {
    printf("%u remaining labels found\n", starting_label-1);
    printf("Mask dimensions: %u %u\n", width, height);
  }
#ifdef AVL_INFO
  //-----------------------------------------------
  //-OPTIONAL:-------------------------------------
  //-GET-MASK-META-INFORMATION---------------------
  //-----------------------------------------------
  struct AVLNode* root = NULL;
  root = get_mask_data(masks, width, height);
  if (!silent) {
    printf("Inorder traversal of AVL tree: ");
    print_label(root);
    printf("\n");
  }
  free_avl_tree_nodes(root);
#endif
  //-----------------------------------------------
  //-CLOSE-UP-SMALL-GAPS-BETWEEN-REGIONS-----------
  //-----------------------------------------------
  TIME(ts_start);
  closeup(&masks, width, height, closeup_pixel_count);
  TIME(ts_end);
  if (!silent) {
    printf("Closing (%lu) up took %f ms\n", closeup_pixel_count, 1000*diff_time(&ts_end, &ts_start));
  }
  //-----------------------------------------------
  //-END-OF-PROCESSING-----------------------------
  //-----------------------------------------------

  //-----------------------------------------------
  //-SAVE-MASK-AS-BINARY-AND-PNG-------------------
  //-----------------------------------------------
  if (masks != NULL) {
    struct bitmap_t* bitmap = uint16_to_bitmap(masks, width, height);
    if (bitmap != NULL) {
      save_png(bitmap, png_file);
      free(bitmap);
    }
    write_array(bin_file, masks, width*height*sizeof(uint16_t));
    free(masks);
  }
  TIME(ts_g_end);
  if (!silent) {
    printf("Finished in %f ms\n", 1000*diff_time(&ts_g_end, &ts_g_start));
  }
  return 0;
}
