#include <lib/seg/mask_data.h>
#include <lib/seg/util.h>

#include <stdio.h>

// Allocate Mask Data for Label
struct MaskData* create_mask_data(uint16_t label)
{
  struct MaskData *data = (struct MaskData*)malloc(sizeof(struct MaskData));
  data->label = label;
  data->area = 0;
  data->perimeter = 0;
  return data;
}

// Compare mask data labels
bool_t compare_labels(struct MaskData* left, struct MaskData* right)
{
  return left->label < right->label;
}

// Create AVL Mask node
struct AVLNode* create_avl_mask_node(struct MaskData* data)
{
  struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
  if (node == NULL) {
    return NULL;
  }
  node->data = data;
  node->compare = (bool_t (*)(void*,void*))&compare_labels;
  node->left = NULL;
  node->right = NULL;
  node->height = 1; // Leaf initially
  return node;
}

// Insert MaskData into the AVL Tree
struct Result insert_mask(struct AVLNode* node, struct MaskData* data)
{
  struct Result result;
  // 1. Standard BST insertion
  if (node == NULL) {
    return (struct Result) {create_avl_mask_node(data), TRUE};
  }

  struct MaskData *node_data = (struct MaskData*)node->data;
  if (node->compare(data, node_data)) {
    result = insert_mask(node->left, data);
    if (!result.success) {
      fprintf(stderr, "Failed to insert!");
      return result;
    }
    node->left = (struct AVLNode*)result.data;
  } else if (node->compare(node->data, data)) {
    result = insert_mask(node->right, data);
    if (!result.success) {
      fprintf(stderr, "Failed to insert!");
      return result;
    }
    node->right = (struct AVLNode*)result.data;
  } else {
    return (struct Result) {node, FALSE};
  }

  // 2. Update height of the ancestor node
  node->height = 1 + max_height(get_height(node->left), get_height(node->right));

  ssize_t balance = get_balance_factor(node);

  // 4. If the node becomes unbalanced

  // LeftLeft
  if ((balance > 1) && node->compare(data, node->left->data)) {
    return (struct Result) {right_rotate(node), TRUE};
  }
  // RightRight
  if ((balance < -1) && node->compare(node->right->data, data)) {
    return (struct Result) {left_rotate(node), TRUE};
  }
  // LeftRight
  if ((balance > 1) && node->compare(node->left->data, data)) {
    return (struct Result) {right_rotate(node), TRUE};
  }
  // RightLeft
  if ((balance < -1) && node->compare(data,node->right->data)) {
    return (struct Result) {left_rotate(node), TRUE};
  }
  return (struct Result) {node, TRUE};
}

// Allocate a label's Mask data in a tree
//  If it already exists, skip the allocation
struct AVLNode* insert_mask_alloc(struct AVLNode* node, uint16_t label)
{
  struct MaskData* data = create_mask_data(label);
  struct Result result = insert_mask(node, data);
  if (!result.success) {
    free(data);
  }
  return (struct AVLNode*)result.data;
}

// Print AVL Node Mask Data Label
void print_label(struct AVLNode* root)
{
  if (root != NULL) {
    print_label(root->left);
    struct MaskData* data = root->data;
    printf("%d: (%zu, %zu) ", data->label, data->area, data->perimeter);
    print_label(root->right);
  }
}

// Increase the label's area
bool_t increase_label_area(struct AVLNode* root, uint16_t label)
{
  if (root == NULL) {
    return FALSE;
  }
  struct MaskData* data = (struct MaskData*)root->data;
  if (data->label == label) {
    data->area++;
  }
  else if (data->label > label) {
    return increase_label_area(root->left, label);
  }
  else if (data->label < label) {
    return increase_label_area(root->right, label);
  }
  return TRUE;
}

// Increase the label's perimeter
bool_t increase_label_perimeter(struct AVLNode* root, uint16_t label)
{
  if (root == NULL) {
    return FALSE;
  }
  struct MaskData* data = (struct MaskData*)root->data;
  if (data->label == label) {
    data->perimeter++;
  }
  else if (data->label > label) {
    return increase_label_perimeter(root->left, label);
  }
  else if (data->label < label) {
    return increase_label_perimeter(root->right, label);
  }
  return TRUE;
}

// Increase the label's area
//  Create an AVL node if it doesn't exist
struct AVLNode* increase_label_area_alloc(struct AVLNode* root, uint16_t label)
{
  struct AVLNode* new_root = root;
  bool_t success = increase_label_area(new_root, label);
  if (success == FALSE) {
    new_root = insert_mask_alloc(new_root, label);
    increase_label_area(new_root, label);
  }
  return new_root;
}

// Increase the label's perimeter
//  Create an AVL node if it doesn't exist
struct AVLNode* increase_label_perimeter_alloc(struct AVLNode* root, uint16_t label)
{
  struct AVLNode* new_root = root;
  bool_t success = increase_label_perimeter(new_root, label);
  if (success == FALSE) {
    new_root = insert_mask_alloc(new_root, label);
    increase_label_perimeter(new_root, label);
  }
  return new_root;
}

// Comparison of uint16_ts
bool_t compare_uint16_t(uint16_t* s1, uint16_t* s2)
{
  return *s1 < *s2;
}

// In-order traversal print pointer
void print_in_order_uint16_t(struct AVLNode* root)
{
  if (root != NULL) {
    print_in_order_uint16_t(root->left);
    printf("%d ", *((uint16_t*)root->data));
    print_in_order_uint16_t(root->right);
  }
}

// Check if uint16_t in AVLTree with uint16_t* data
bool_t in_uint16_t_tree(struct AVLNode* root, uint16_t value)
{
  if (root == NULL) {
    return FALSE;
  }
  if (*((uint16_t*)root->data) == value) {
    return TRUE;
  } else if (value < *((uint16_t*)root->data)) {
    return in_uint16_t_tree(root->left, value);
  } else {
    return in_uint16_t_tree(root->right, value);
  }
}

// Filter out small masks
//  Assumption: Contiguous labeling
struct AVLNode* get_small_labels(struct AVLNode* removal_tree, struct AVLNode* label_tree, size_t min_area, size_t min_perimeter)
{
  struct AVLNode* return_tree = removal_tree;
  if (label_tree != NULL) {
    return_tree = get_small_labels(return_tree, label_tree->left, min_area, min_perimeter);
    struct MaskData* node_data = (struct MaskData*)label_tree->data;
    if ((node_data->area < min_area) || (node_data->perimeter < min_perimeter)) {
      // Insert
      struct Result result = avl_insert(return_tree, &node_data->label, (bool_t (*)(void*,void*))compare_uint16_t);
      if (result.success) {
	return_tree = result.data;
      }
    }
    return_tree = get_small_labels(return_tree, label_tree->right, min_area, min_perimeter);
  }
  return return_tree;
}

// Get mask label data
struct AVLNode* get_mask_data(uint16_t* masks, uint32_t width, uint32_t height)
{
  struct AVLNode* root = NULL;
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      if (masks[coord] != 0) {
	root = increase_label_area_alloc(root, masks[coord]);
	if (is_on_mask_boundary(masks, width, height, x, y)) {
	  increase_label_perimeter(root, masks[coord]);
	}
      }
    }
  }
  return root;
}

// Filter out small masks in mask
void filter_small_masks(uint16_t* masks, uint32_t width, uint32_t height, size_t min_area, size_t min_perimeter)
{
  struct AVLNode* root = get_mask_data(masks, width, height);
  struct AVLNode* small_label_tree = NULL;
  small_label_tree = get_small_labels(NULL, root, min_area, min_perimeter);
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      size_t coord = x + y*width;
      if (in_uint16_t_tree(small_label_tree, masks[coord])) {
	masks[coord] = 0;
      }
    }
  }
  free_avl_tree(small_label_tree);
  free_avl_tree_nodes(root);
}
