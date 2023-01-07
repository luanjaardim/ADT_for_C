#include "gnode.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

/*
* each node has a type of StdDataType, and can be linked with nodes of another type
*
*/
typedef struct GNode {
  void *data;
  size_t data_size;
  StdDataTypes type;
  struct GNode *next, *prev;
} GNode;

/*
* By the type informed we define the size of the data
*
*/
size_t define_type(StdDataTypes type) {
  if(type == CHAR_TYPE) return sizeof(char);
  if(type == SHORT_TYPE) return sizeof(short);
  if(type == INT_TYPE) return sizeof(int);
  if(type == FLOAT_TYPE) return sizeof(float);
  if(type == DOUBLE_TYPE) return sizeof(double);
  if(type == LONG_LONG_TYPE) return sizeof(long long);
  if(type == POINTER_TYPE) return sizeof(void *);
}

/*
* return a new node with that contains value and points to next_node
*
*/
GNode *gnode_create(void *value, StdDataTypes type, GNode *next_node, GNode *prev_node) {
  if(value == NULL) handle_error("trying to create a node with a NULL value");

  GNode *new_node = (GNode *) malloc(sizeof(GNode));
  if(new_node == NULL) handle_error("fail to malloc GNode");

  size_t data_size = define_type(type);

  new_node->data = malloc(data_size);
  if(new_node == NULL) handle_error("fail to malloc data");

  memcpy(new_node->data, value, data_size);

  new_node->next = next_node;
  new_node->prev = prev_node;
  new_node->type = type;
  new_node->data_size = data_size;

  return new_node;
}

/*
* creating a node without value
*
*/
GNode *gnode_create_null() {
  GNode *new_node = (GNode *) malloc(sizeof(GNode));
  if(new_node == NULL) handle_error("fail to malloc GNode");

  new_node->data = calloc(sizeof(char), 1);
  if(new_node == NULL) handle_error("fail to calloc data");
  
  new_node->next = NULL;
  new_node->prev = NULL;
  new_node->type = CHAR_TYPE;
  new_node->data_size = 1;
  return new_node;
}

/*
* deallocate the passed node
*
*/
void gnode_delete(GNode *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");
  if(n->data == NULL) handle_error("trying to delete a NULL data pointer");
  free(n->data);
  n->data = NULL;
  n->next = NULL;
  n->prev = NULL;
  free(n);
  n = NULL;
}

/*
* deallocate the node passed and all the other nodes that comes after it
*
*/
void gnode_delete_recursive(GNode *n) {
  if(n == NULL) return;
  gnode_delete_recursive(n->next);
  gnode_delete(n);
}

/*
* returns the next node of n
*
*/
GNode *gnode_get_next(GNode *n) {
  if(n == NULL) handle_error("trying to get next of a NULL address");
  return n->next;
}

/*
* returns the previous node of n
*
*/
GNode *gnode_get_prev(GNode *n) {
  if(n == NULL) handle_error("trying to get next of a NULL address");
  return n->prev;
}

/*
* set the next node of n and return the previous one
*
*/
GNode *gnode_set_next(GNode *n, GNode *new_next) {
  if(n == NULL) handle_error("trying to set next of a NULL address");

  GNode *old_next = n->next;
  n->next = new_next;
  return old_next;
}

/*
* set the prev node of n and return the previous one
*
*/
GNode *gnode_set_prev(GNode *n, GNode *new_prev) {
  if(n == NULL) handle_error("trying to set next of a NULL address");

  GNode *old_prev = n->prev;
  n->prev = new_prev;
  return old_prev;
}

/*
* copy the value of data to to_ret
*
*/
void gnode_get_value(GNode *n, void *to_ret) {
  if(n == NULL) handle_error("trying to get value of a NULL address");
  if(to_ret == NULL) handle_error("trying to copy a value to a NULL address");
  memcpy(to_ret, n->data, n->data_size);
}

/*
* set the value of data with to_set and copy the previous one to to_ret, the data type don't need to be the same
*
*/
void gnode_set_value(GNode *n, void *to_ret, void *to_set, StdDataTypes new_type) {
  if(n == NULL) handle_error("trying to set value of a NULL address");
  if(to_ret == NULL or to_set == NULL) handle_error("tryingo to use NULL addresses for set value");
  memcpy(to_ret, n->data, n->data_size);
  
  size_t new_data_size = define_type(new_type);

  n->data = realloc(n->data, new_data_size);
  if(n->data == NULL) handle_error("fail to realloc data");
  memcpy(n->data, to_set, new_data_size);
  n->data_size = new_data_size;
  n->type = new_type;
}

size_t gnode_data_size(GNode *n) {
  if(n == NULL) handle_error("trying to return the data_size of a NULL address");
  return n->data_size;
}

StdDataTypes gnode_get_type(GNode *n) {
  if(n == NULL) handle_error("trying to return the type of a NULL address");
  return n->type;
}