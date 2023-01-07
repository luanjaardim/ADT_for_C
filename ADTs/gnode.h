#ifndef GNODE_LIB_DEF
#define GNODE_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct GNode GNode;

typedef enum StdDataTypes {
  CHAR_TYPE,
  SHORT_TYPE,
  INT_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  LONG_LONG_TYPE,
  POINTER_TYPE,
} StdDataTypes;

GNode *gnode_create(void *value, StdDataTypes type, GNode *next_node, GNode *prev_node);
GNode *gnode_create_null();
void gnode_delete(GNode *n);
void gnode_delete_recursive(GNode *n);
GNode *gnode_get_next(GNode *n);
GNode *gnode_get_prev(GNode *n);
GNode *gnode_set_next(GNode *n, GNode *new_next);
GNode *gnode_set_prev(GNode *n, GNode *new_prev);
void gnode_get_value(GNode *n, void *to_ret);
void gnode_set_value(GNode *n, void *to_ret, void *to_set, StdDataTypes new_type);
size_t gnode_data_size(GNode *n);
StdDataTypes gnode_get_type(GNode *n);

#endif /* GNODE_LIB_DEF */