#include <lib/algo/avl_tree.h>
#include <lib/monad.h>

#include <stddef.h>
#include <stdio.h>

// Get the height of an AVL node
uint8_t get_height(struct AVLNode* node)
{
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

// Get the Maximum Height between two
uint8_t max_height(uint8_t a, uint8_t b)
{
  return (a > b) ? a : b;
}

// Get the balance factor of a node
ssize_t get_balance_factor(struct AVLNode* node)
{
  if (node == NULL) {
    return 0;
  }
  return get_height(node->left) - get_height(node->right);
}

// Rotate an AVL node right
struct AVLNode* right_rotate(struct AVLNode* parent)
{
  struct AVLNode* child1 = parent->left;
  struct AVLNode* child2 = child1->right;

  child1->right = parent;
  parent->left = child2;

  parent->height = max_height(get_height(parent->left), get_height(parent->right)) + 1;
  child1->height = max_height(get_height(child1->left), get_height(child1->right)) + 1;
  return child1;
}

// Rotate an AVL node left
struct AVLNode* left_rotate(struct AVLNode* parent)
{
  struct AVLNode* child1 = parent->right;
  struct AVLNode* child2 = child1->left;

  child1->left = parent;
  parent->right = child2;

  parent->height = max_height(get_height(parent->left), get_height(parent->right)) + 1;
  child1->height = max_height(get_height(child1->left), get_height(child1->right)) + 1;
  return child1;
}

// Create AVL node
struct AVLNode* create_avl_node(void* data, bool_t (*compare)(void*, void*))
{
  struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
  if (node == NULL) {
    return NULL;
  }
  node->data = data;
  node->compare = compare;
  node->left = NULL;
  node->right = NULL;
  node->height = 1; // Leaf initially
  return node;
}

// Insert data into AVL tree
struct Result avl_insert(struct AVLNode* node, void* data, bool_t (*compare)(void*, void*))
{
  struct Result result;
  // 1. Standard BST insertion
  if (node == NULL) {
    return (struct Result) {create_avl_node(data, compare), TRUE};
  }

  struct MaskData *node_data = (struct MaskData*)node->data;
  if (node->compare(data, node_data)) {
    result = avl_insert(node->left, data, compare);
    if (!result.success) {
      fprintf(stderr, "Failed to insert!");
      return result;
    }
    node->left = (struct AVLNode*)result.data;
  } else if (node->compare(node->data, data)) {
    result = avl_insert(node->right, data, compare);
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

// In-order traversal print pointer
void print_in_order(struct AVLNode* root)
{
  if (root != NULL) {
    print_in_order(root->left);
    printf("%p ", root->data);
    print_in_order(root->right);
  }
}

// Free avl tree nodes starting at root
void free_avl_tree(struct AVLNode* root)
{
  if (root != NULL) {
    free_avl_tree(root->left);
    free_avl_tree(root->right);
    free(root);
  }
}

// Free avl tree and their data starting at root
void free_avl_tree_nodes(struct AVLNode* root)
{
  if (root != NULL) {
    free_avl_tree_nodes(root->left);
    free_avl_tree_nodes(root->right);
    if (root->data != NULL) {
      free(root->data);
    }
    free(root);
  }
}
