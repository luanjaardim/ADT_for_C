#ifndef NODE_LIB_DEF
#define NODE_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct Node Node;

typedef enum StdDataTypes {
  CHAR_TYPE,
  SHORT_TYPE,
  INT_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  LONG_LONG_TYPE,
  POINTER_TYPE,
} StdDataTypes;

Node *node_create(void *value, StdDataTypes type, Node *next_node, Node *prev_node);
Node *node_create_null();
void node_delete(Node *n);
void node_delete_recursive(Node *n);
Node *node_get_next(Node *n);
Node *node_get_prev(Node *n);
Node *node_set_next(Node *n, Node *new_next);
Node *node_set_prev(Node *n, Node *new_prev);
void node_get_value(Node *n, void *to_ret);
void node_set_value(Node *n, void *to_ret, void *to_set, StdDataTypes new_type);
size_t node_data_size(Node *n);
StdDataTypes node_get_type(Node *n);

#endif /* NODE_LIB_DEF */