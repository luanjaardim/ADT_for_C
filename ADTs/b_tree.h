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

B_Tree *btree_create(int (*cmp)(StdPairKV *, StdPairKV *, size_t));
void btree_delete(B_Tree *bt);

#ifndef INIT_B_TREE_TYPE
#define INIT_B_TREE_TYPE(name, key_type, value_type) \
  size_t name##_key_size = sizeof(key_type), name##_value_size = sizeof(value_type);
#endif /* INIT_B_TREE_TYPE */

#endif /* B_TREE_LIB_DEF */