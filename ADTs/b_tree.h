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

typedef struct B_Tree B_Tree;
typedef struct StdPairKV StdPairKV;

B_Tree *btree_create(void *key_root, size_t key_size, void *value_root, size_t value_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t));
void btree_delete(B_Tree *bt);
void btree_insert(B_Tree *bt, void *new_key, size_t key_size, void *new_value, size_t value_size);
int btree_get_value(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size);
int btree_remove(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size);
unsigned get_num_pairs(B_Tree *bt);
void btree_pairs_visit(B_Tree *bt, void (*custom_func)(StdPairKV *));
StdPairKV *btree_get_pair(B_Tree *bt, void *key, size_t key_size);
void pair_get_key(StdPairKV *pair, void *key_to_ret, size_t key_size);
void pair_get_value(StdPairKV *pair, void *value_to_ret, size_t value_size);
int btree_std_cmp(StdPairKV *f, StdPairKV *s, size_t key_size);

//defining some macros to help with the use of the functions
#ifndef INIT_B_TREE_TYPE
#define INIT_B_TREE_TYPE(name, key_type, value_type) \
  size_t name##_key_size = sizeof(key_type), name##_value_size = sizeof(value_type); \
  B_Tree *name##_btree_create(key_type key_root, value_type value_root, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) { \
    return btree_create((void *) &key_root, name##_key_size, (void *) &value_root, name##_value_size, cmp); \
  } \
  void name##_btree_insert(B_Tree *bt, key_type new_key, value_type new_value) { \
    btree_insert(bt, (void *) &new_key, name##_key_size, (void *) &new_value, name##_value_size); \
  } \
  int name##_btree_remove(B_Tree *bt, key_type key, value_type *to_ret) { \
    return btree_remove(bt, (void *) &key, name##_key_size, (void *) to_ret, name##_value_size); \
  } \
  int name##_btree_get_value(B_Tree *bt, key_type key, value_type *to_ret) { \
    return btree_get_value(bt, (void *) &key, name##_key_size, (void *) to_ret, name##_value_size); \
  } \
  StdPairKV *name##_btree_get_pair(B_Tree *bt, key_type key) { \
    return btree_get_pair(bt, (void *) &key, name##_key_size); \
  } \
  key_type name##_pair_get_key(StdPairKV *pair) { \
    key_type val; \
    pair_get_key(pair, (void *) &val, name##_key_size); \
    return val; \
  } \
  value_type name##_pair_get_value(StdPairKV *pair) { \
    value_type val; \
    pair_get_value(pair, (void *) &val, name##_value_size); \
    return val; \
  }


#endif /* INIT_B_TREE_TYPE */

#endif /* B_TREE_LIB_DEF */