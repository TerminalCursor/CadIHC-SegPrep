#ifndef INC_LIB_ALGO_AVL_TREE_H
#define INC_LIB_ALGO_AVL_TREE_H

#include <lib/bool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

struct AVLNode {
  void* data;
  bool_t (*compare)(void*, void*);
  struct AVLNode* left;
  struct AVLNode* right;
  uint8_t height;
};

// Get the height of an AVL node
uint8_t get_height(struct AVLNode* node);

// Get the Maximum Height between two
uint8_t max_height(uint8_t a, uint8_t b);

// Get the balance factor of a node
ssize_t get_balance_factor(struct AVLNode* node);

// Rotate an AVL node right
struct AVLNode* right_rotate(struct AVLNode* parent);

// Rotate an AVL node left
struct AVLNode* left_rotate(struct AVLNode* parent);

// Create AVL node
struct AVLNode* create_avl_node(void* data, bool_t (*compare)(void*, void*));

// Insert data into AVL tree
struct Result avl_insert(struct AVLNode* node, void* data, bool_t (*compare)(void*, void*));

// In-order traversal print pointer
void print_in_order(struct AVLNode* root);

// Free avl tree nodes starting at root
void free_avl_tree(struct AVLNode* root);

// Free avl tree and their data starting at root
void free_avl_tree_nodes(struct AVLNode* root);

#endif
