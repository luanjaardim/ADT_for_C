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

Node *node_create(void *value, size_t data_size);
Node *node_create_null(size_t data_size);
void node_delete(Node *n);
unsigned node_delete_recursive(Node *n);
void node_get_value(Node *n, void *to_ret, size_t data_size);
void node_set_value(Node *n, void *to_ret, void *to_add, size_t data_size);
void node_set_link(Node *n, Node *n2);
void node_set_link_at(Node *n, Node *n2, unsigned position);
void node_set_double_link(Node *n, Node *n2);
void node_set_double_link_at(Node *n, Node *n2, unsigned position_n, unsigned position_n2);
Node *node_get_neighbour(Node *n, unsigned neighbour_num);
Node *node_swap_neighbour_position(Node *n, unsigned first_position, unsigned second_position);
unsigned node_get_num_neighbours(Node *n);


// Node *node_get_next(Node *n);
// Node *node_get_prev(Node *n);
// Node *node_set_next(Node *n, Node *new_next);
// Node *node_set_prev(Node *n, Node *new_prev);
// void node_get_value(Node *n, void *to_ret);
// void node_set_value(Node *n, void *to_ret, void *to_set, StdDataTypes new_type);
// size_t node_data_size(Node *n);
// StdDataTypes node_get_type(Node *n);

#ifndef INIT_NODE_TYPE
#define INIT_NODE_TYPE(name, type) \
  size_t name##_data_size = sizeof(type); \
  Node *name##_node_create(type value) { \
    return node_create((void *) &value, name##_data_size); \
  } \
  Node *name##_node_create_null() { \
    return node_create_null(name##_data_size); \
  } \
  type name##_node_get_value(Node *n) { \
    type val; \
    node_get_value(n, (void *) &val, name##_data_size); \
    return val; \
  } \
  type name##_node_set_value(Node *n, type value) { \
    type val; \
    node_set_value(n, (void *) &val, (void *) &value, name##_data_size); \
    return val; \
  }

#endif /* INIT_NODE_TYPE */

#endif /* NODE_LIB_DEF */