#include "b_tree.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct B_Tree {
  Node *root;
  unsigned vertices;
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

Node *btree_node_create(void *key, size_t key_size, void *value, size_t value_size) {
  StdPairKV *node_pair = (StdPairKV *) malloc(sizeof(StdPairKV));
  if(node_pair == NULL) handle_error("fail to malloc root pair");
  node_pair->key = malloc(key_size);
  if(node_pair->key == NULL) handle_error("fail to malloc root key's");
  memcpy(node_pair->key, key, key_size);
  node_pair->value = malloc(value_size);
  if(node_pair->value == NULL) handle_error("fail to malloc root value's");
  memcpy(node_pair->value, value, value_size);

  size_t data_size = sizeof(StdPairKV *);
  Node *new_node = node_create(&node_pair, data_size);
  for(int link = 0; link < 3; link++) node_set_link(new_node, NULL); // at the moment it hasn't a parent, left child and right child

  return new_node;
}

B_Tree *btree_create(void *key_root, size_t key_size, void *value_root, size_t value_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  B_Tree *new_tree = (B_Tree *) malloc(sizeof(B_Tree));
  if(new_tree == NULL) handle_error("fail to malloc binary tree");

  new_tree->root = btree_node_create(key_root, key_size, value_root, value_size);
  new_tree->vertices = 1;
  if(cmp == NULL) new_tree->cmp = btree_std_cmp;
  else new_tree->cmp = cmp;

  return new_tree;
}

void btree_delete_pair(Node *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");
  StdPairKV *pair;
  node_get_value(n, &pair, sizeof(StdPairKV *));
  free(pair->key);
  pair->key = NULL;
  free(pair->value);
  pair->value = NULL;
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

void pair_get_key(StdPairKV *pair, void *key_to_ret, size_t key_size) {
  if(pair == NULL) handle_error("trying to get key from a NULL pair");
  memcpy(key_to_ret, pair->key, key_size);
}

void pair_get_value(StdPairKV *pair, void *value_to_ret, size_t value_size) {
  if(pair == NULL) handle_error("trying to get value from a NULL pair");
  memcpy(value_to_ret, pair->value, value_size);
}

void pair_get_key_and_value(StdPairKV *pair, void *key_to_ret, void *value_to_ret, size_t key_size, size_t value_size) {
  pair_get_key(pair, key_to_ret, key_size);
  pair_get_value(pair, value_to_ret, value_size);
}

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

Node *btree_insert_aux(Node *node, Node *to_insert, size_t key_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  StdPairKV *pair_node, *pair_to_insert;
  node_get_value(node, &pair_node, sizeof(StdPairKV *));
  node_get_value(to_insert, &pair_to_insert, sizeof(StdPairKV *));

  int result = cmp(pair_to_insert, pair_node, key_size);
  if(!result) {
    btree_delete_pair(node);
    node_set_value(node, &pair_node, &pair_to_insert, sizeof(StdPairKV *));
    pair_node = NULL;
    node_delete(to_insert, NULL);
  }
  else if(result > 0) {
    Node *right = node_get_neighbour(node, RIGHT);
    if(right == NULL) 
      node_set_double_link_at(node, to_insert, RIGHT, PARENT);
    else
      btree_insert_aux(right, to_insert, key_size, cmp);
  }
  else {
    Node *left = node_get_neighbour(node, LEFT);
    if(left == NULL) 
      node_set_double_link_at(node, to_insert, LEFT, PARENT);
    else
      btree_insert_aux(left, to_insert, key_size, cmp);
  }
}

void btree_insert(B_Tree *bt, void *new_key, size_t key_size, void *new_value, size_t value_size) {
  Node *to_insert = btree_node_create(new_key, key_size, new_value, value_size);
  btree_insert_aux(bt->root, to_insert, key_size, bt->cmp);
  bt->vertices++;
}

Node *search_aux(Node *node, StdPairKV *pair, size_t key_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  StdPairKV *node_pair;
  node_get_value(node, &node_pair, sizeof(StdPairKV *));

  int result = cmp(pair, node_pair, key_size);
  if(!result) return node;
  if(result > 0) {
    Node *right = node_get_neighbour(node, RIGHT);
    if(right == NULL) return NULL;
    else return search_aux(right, pair, key_size, cmp);
  }
  else {
    Node *left = node_get_neighbour(node, LEFT);
    if(left == NULL) return NULL;
    else return search_aux(left, pair, key_size, cmp);
  }
}

Node *btree_search_key(B_Tree *bt, void *key, size_t key_size) {
  StdPairKV *pair = (StdPairKV *) malloc(sizeof(StdPairKV));
  if(pair == NULL) handle_error("fail to create pair");
  pair->key = key;
  Node *found_node = search_aux(bt->root, pair, key_size, bt->cmp);
  free(pair);
  return found_node;
}

int btree_get_value(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size) {
  Node *found_node = btree_search_key(bt, key, key_size);
  if(found_node == NULL) return -1;
  StdPairKV *pair;
  node_get_value(found_node, &pair, sizeof(StdPairKV *));
  pair_get_value(pair, to_ret, value_size);
  return 0;
}