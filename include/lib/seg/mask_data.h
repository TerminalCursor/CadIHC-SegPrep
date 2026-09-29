#ifndef INC_LIB_SEG_MASK_DATA_H
#define INC_LIB_SEG_MASK_DATA_H

#include <lib/algo/avl_tree.h>
#include <lib/monad.h>

struct MaskData {
  uint16_t label;
  size_t area;
  size_t perimeter;
};

// Allocate Mask Data for Label
struct MaskData* create_mask_data(uint16_t label);

// Compare mask data labels
bool_t compare_labels(struct MaskData* left, struct MaskData* right);

// Create AVL Mask node
struct AVLNode* create_avl_mask_node(struct MaskData* data);

// Insert MaskData into the AVL Tree
struct Result insert_mask(struct AVLNode* node, struct MaskData* data);

// Allocate a label's Mask data in a tree
//  If it already exists, skip the allocation
struct AVLNode* insert_mask_alloc(struct AVLNode* node, uint16_t label);

// Print AVL Node Mask Data Label
void print_label(struct AVLNode* root);

// Increase the label's area
bool_t increase_label_area(struct AVLNode* root, uint16_t label);

// Increase the label's perimeter
bool_t increase_label_perimeter(struct AVLNode* root, uint16_t label);

// Increase the label's area
//  Create an AVL node if it doesn't exist
struct AVLNode* increase_label_area_alloc(struct AVLNode* root, uint16_t label);

// Increase the label's perimeter
//  Create an AVL node if it doesn't exist
struct AVLNode* increase_label_perimeter_alloc(struct AVLNode* root, uint16_t label);

// Comparison of uint16_ts
bool_t compare_uint16_t(uint16_t* s1, uint16_t* s2);

// In-order traversal print pointer
void print_in_order_uint16_t(struct AVLNode* root);

// Check if uint16_t in AVLTree with uint16_t* data
bool_t in_uint16_t_tree(struct AVLNode* root, uint16_t value);

// Filter out small masks
//  Assumption: Contiguous labeling
struct AVLNode* get_small_labels(struct AVLNode* removal_tree, struct AVLNode* label_tree, size_t min_area, size_t min_perimeter);

// Get mask label data
struct AVLNode* get_mask_data(uint16_t* masks, uint32_t width, uint32_t height);

// Filter out small masks in mask
void filter_small_masks(uint16_t* masks, uint32_t width, uint32_t height, size_t min_area, size_t min_perimeter);

#endif
