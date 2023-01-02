#include "b_tree.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct B_Tree {
  Node *root;
  unsigned num_nodes;
} B_Tree;

typedef struct StdPairKV {
  void *key, *value;
  unsigned key_data_len, value_data_len;
} StdPairKV;

typedef enum NodeChilds {
  LEFT,
  RIGHT,
} NodeChilds;

B_Tree *btree_create() {
  B_Tree *new_tree = (B_Tree *) malloc(sizeof(B_Tree));
  if(new_tree == NULL) handle_error("fail to malloc binary tree");

  size_t data_size = sizeof(StdPairKV *);
  StdPairKV *root_pair = (StdPairKV *) calloc(sizeof(StdPairKV), 1);
  if(root_pair == NULL) handle_error("fail to malloc root_pair");

  new_tree->root = node_create((void *) &root_pair, data_size);
  new_tree->num_nodes = 0;

  return new_tree;
}

void btree_delete_pair(Node *n) {
  StdPairKV *pair;
  node_get_value(n, &pair, sizeof(StdPairKV *));
  if(pair->key != NULL) {
    free(pair->key);
    pair->key = NULL;
  }
  if(pair->value != NULL) {
    free(pair->value);
    pair->value = NULL;
  }
  free(pair);
  pair = NULL;
}

void btree_delete(B_Tree *bt) {
  if(bt == NULL) handle_error("trying to delete a NULL address");

  node_delete_recursive(bt->root, btree_delete_pair);
  bt->root = NULL;
  free(bt);
  bt = NULL;
}