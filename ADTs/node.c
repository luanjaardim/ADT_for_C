#include "node.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct Node {
  void *data;
  size_t data_size;
  StdDataTypes type;
  struct Node *next;
} Node;

/*
* return a new node with that contains value and points to next_node
*
*/
Node *node_create(void *value, size_t data_size, StdDataTypes type, Node *next_node) {
  if(value == NULL) handle_error("trying to create a node with a NULL value");

  Node *new_node = (Node *) malloc(sizeof(Node));
  if(new_node == NULL) handle_error("fail to malloc Node");

  new_node->data = malloc(data_size);
  memcpy(new_node->data, value, data_size);

  new_node->next = next_node;
  new_node->data_size = data_size;
  new_node->type = type;

  return new_node;
}

/*
* creating a node without value
*
*/
Node *node_create_null() {
  Node *new_node = (Node *) malloc(sizeof(Node));
  if(new_node == NULL) handle_error("fail to malloc Node");

  new_node->data = calloc(sizeof(char), 1);
  if(new_node == NULL) handle_error("fail to calloc data");
  
  new_node->next = NULL;
  new_node->data_size = 1;
  new_node->type = CHAR_TYPE;
  return new_node;
}

/*
* deallocate the passed node
*
*/
void node_delete(Node *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");
  if(n->data == NULL) handle_error("trying to delete a NULL data pointer");
  free(n->data);
  n->data = NULL;
  free(n);
  n = NULL;
}

/*
* deallocate the node passed and all the other nodes that comes after it
*
*/
void node_delete_recursive(Node *n) {
  if(n == NULL) return;
  node_delete_recursive(n->next);
  node_delete(n);
}

/*
* returns the next node of n
*
*/
Node *node_get_next(Node *n) {
  if(n == NULL) handle_error("trying to get next of a NULL address");
  return n->next;
}

/*
* set the next node of n and return the previous one
*
*/
Node *node_set_next(Node *n, Node *new_next) {
  if(n == NULL) handle_error("trying to set next of a NULL address");

  Node *previous_next = n->next;
  n->next = new_next;
  return previous_next;
}

/*
* copy the value of data to to_ret
*
*/
void node_get_value(Node *n, void *to_ret) {
  if(n == NULL) handle_error("trying to get value of a NULL address");
  if(to_ret == NULL) handle_error("trying to copy a value to a NULL address");
  memcpy(to_ret, n->data, n->data_size);
}

/*
* set the value of data with to_set and copy the previous one to to_ret, the data type don't need to be the same
*
*/
void node_set_value(Node *n, void *to_ret, void *to_set, size_t new_data_size, StdDataTypes new_type) {
  if(n == NULL) handle_error("trying to set value of a NULL address");
  if(to_ret == NULL or to_set == NULL) handle_error("tryingo to use NULL addresses for set value");
  memcpy(to_ret, n->data, n->data_size);
  
  n->data = realloc(n->data, new_data_size);
  if(n->data == NULL) handle_error("fail to realloc data");
  memcpy(n->data, to_set, new_data_size);
  n->data_size = new_data_size;
  n->type = new_type;
}

size_t node_data_size(Node *n) {
  if(n == NULL) handle_error("trying to return the data_size of a NULL address");
  return n->data_size;
}

StdDataTypes node_get_type(Node *n) {
  if(n == NULL) handle_error("trying to return the type of a NULL address");
  return n->type;
}