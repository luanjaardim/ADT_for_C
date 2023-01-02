#include "b_tree.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct B_Tree {
  Node *root;
  unsigned num_nodes;
  int (*cmp)(StdPairKV *, StdPairKV *, size_t);
} B_Tree;

typedef struct StdPairKV {
  void *key, *value;
} StdPairKV;

typedef enum LinkedNodes {
  PARENT,
  LEFT,
  RIGHT,
} LinkedNodes;

int btree_std_cmp(StdPairKV *f, StdPairKV *s, size_t key_size) {
  u_int8_t bytes_f[key_size];
  u_int8_t bytes_s[key_size];

  memcpy(bytes_f, f->key, key_size);
  memcpy(bytes_s, s->key, key_size);

  for(int byte = key_size - 1; byte >= 0; byte--) {
    if(bytes_f[byte] == bytes_s[byte]) continue;
    if(bytes_f[byte] > bytes_s[byte]) return 1; //greater
    else return -1; //lesser
  }
  return 0; //equal
}

B_Tree *btree_create(int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  B_Tree *new_tree = (B_Tree *) malloc(sizeof(B_Tree));
  if(new_tree == NULL) handle_error("fail to malloc binary tree");

  size_t data_size = sizeof(StdPairKV *);
  StdPairKV *root_pair = (StdPairKV *) calloc(sizeof(StdPairKV), 1);
  if(root_pair == NULL) handle_error("fail to malloc root_pair");

  new_tree->root = node_create((void *) &root_pair, data_size);
  new_tree->num_nodes = 0;
  if(cmp == NULL) new_tree->cmp = btree_std_cmp;
  else new_tree->cmp = cmp;

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