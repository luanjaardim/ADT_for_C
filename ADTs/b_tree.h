#ifndef B_TREE_LIB_DEF
#define B_TREE_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

#include "node.h"

typedef struct B_Tree B_Tree;
typedef struct StdPairKV StdPairKV;

// Node *test(B_Tree *bt);

B_Tree *btree_create(void *key_root, size_t key_size, void *value_root, size_t value_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t));
void btree_delete(B_Tree *bt);
void btree_insert(B_Tree *bt, void *new_key, size_t key_size, void *new_value, size_t value_size);
int btree_get_value(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size);
int btree_std_cmp(StdPairKV *f, StdPairKV *s, size_t key_size);

#ifndef INIT_B_TREE_TYPE
#define INIT_B_TREE_TYPE(name, key_type, value_type) \
  size_t name##_key_size = sizeof(key_type), name##_value_size = sizeof(value_type); \
  B_Tree *name##_btree_create(key_type key_root, value_type value_root, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) { \
    return btree_create((void *) &key_root, name##_key_size, (void *) &value_root, name##_value_size, cmp); \
  } \
  void name##_btree_insert(B_Tree *bt, key_type new_key, value_type new_value) { \
    btree_insert(bt, (void *) &new_key, name##_key_size, (void *) &new_value, name##_value_size); \
  } \
  int name##_btree_get_value(B_Tree *bt, key_type key, value_type *to_ret) { \
    return btree_get_value(bt, (void *) &key, name##_key_size, (void *) to_ret, name##_value_size); \
  }

#endif /* INIT_B_TREE_TYPE */

#endif /* B_TREE_LIB_DEF */