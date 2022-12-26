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

Node *node_create(void *value, size_t data_size, Node *next_node);
Node *node_create_null();
void node_delete(Node *n);
void node_delete_recursive(Node *n);
Node *node_get_next(Node *n);
Node *node_set_next(Node *n, Node *new_next);
void node_get_value(Node *n, void *to_ret);
void node_set_value(Node *n, void *to_ret, void *to_set, size_t new_data_size);
size_t node_data_size(Node *n);

#endif /* NODE_LIB_DEF */