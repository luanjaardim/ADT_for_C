#include "node.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

#define INITIAL_CAP 2
unsigned num_nodes = 0;

typedef struct Node {
  void *data;
  unsigned id;
  unsigned num_neighbours;
  unsigned cap_neighbours;
  struct Node **neighbours;
} Node;

/*
* return a new node with that contains value and points to next_node
*
*/
Node *node_create(void *value, size_t data_size) {
  if(value == NULL) handle_error("trying to create a node with a NULL value");

  Node *new_node = (Node *) malloc(sizeof(Node));
  if(new_node == NULL) handle_error("fail to malloc Node");

  new_node->data = malloc(data_size);
  if(new_node->data == NULL) handle_error("fail to malloc data");

  new_node->neighbours = (Node **) malloc(sizeof(Node *) * INITIAL_CAP);
  if(new_node->neighbours == NULL) handle_error("fail to malloc node neighbours");

  new_node->cap_neighbours = INITIAL_CAP;
  new_node->num_neighbours = 0;
  new_node->id = num_nodes++;
  memcpy(new_node->data, value, data_size);

  return new_node;
}

/*
* creating a node without value (0)
*
*/
Node *node_create_null(size_t data_size) {
  Node *new_node = (Node *) malloc(sizeof(Node));
  if(new_node == NULL) handle_error("fail to malloc Node");

  new_node->data = calloc(data_size, 1);
  if(new_node == NULL) handle_error("fail to calloc data");
  
  new_node->neighbours = (Node **) malloc(sizeof(Node *) * INITIAL_CAP);
  if(new_node->neighbours == NULL) handle_error("fail to malloc node neighbours");

  new_node->cap_neighbours = INITIAL_CAP;
  new_node->num_neighbours = 0;
  new_node->id = num_nodes++;

  return new_node;
}

/*
* deallocate the passed node, not recommended, instead use node_delete_recursive
* if it's used with delete_recursive a double free can occur
* if using, all nodes must be delete
*/
void node_delete(Node *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");

  free(n->neighbours);
  n->neighbours = NULL;
  free(n->data);
  n->data = NULL;
  free(n);
  n = NULL;
}

void node_delete_recursive_aux(Node *n, u_int8_t visited[], int *deleted_nodes) {
  visited[n->id] = 1;
  Node *tmp;
  for(int i=0; i<n->num_neighbours; i++) {
    tmp = n->neighbours[i];
    if(visited[tmp->id]) continue;
    node_delete_recursive_aux(tmp, visited, deleted_nodes);
    n->neighbours[i] = NULL;
  }
  (*deleted_nodes)++;
  node_delete(n);
}

/*
* deletes all nodes of a tree and returns the number of deleted nodes
*
*/
unsigned node_delete_recursive(Node *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");

  u_int8_t visited[num_nodes];
  for(int i=0; i<num_nodes; i++) visited[i] = 0;
  int deleted_nodes = 0;
  node_delete_recursive_aux(n, visited, &deleted_nodes);
  return deleted_nodes;
}

/*
* copy the value on data of n to to_ret
*
*/
void node_get_value(Node *n, void *to_ret, size_t data_size) {
  if(n == NULL or to_ret == NULL) handle_error("trying to get value with NULL addresses");
  memcpy(to_ret, n->data, data_size);
}

/*
* copy the previous value of n on data to to_ret and set a new one with to_add
* both must have the same type(data_size)
*/
void node_set_value(Node *n, void *to_ret, void *to_add, size_t data_size) {
  if(n == NULL or to_ret == NULL or to_add == NULL) handle_error("trying to set value with NULL addresses");
  memcpy(to_ret, n->data, data_size);
  memcpy(n->data, to_add, data_size);
}

/*
* realloc neighbours if needed
*
*/
void maybe_realloc_neighbours(Node *n){
  if(n->num_neighbours == n->cap_neighbours) {
    n->cap_neighbours <<= 1;
    n->neighbours = (Node **) realloc(n->neighbours, sizeof(Node *) * n->cap_neighbours);
    if(n->neighbours == NULL) handle_error("fail to realloc neighbours");
  }
}

/*
* linking n with n2
*
*/
void node_set_link(Node *n, Node *n2) {
  if(n == NULL or n2 == NULL) handle_error("trying to link with NULL nodes");
  maybe_realloc_neighbours(n);
  n->neighbours[n->num_neighbours++] = n2;
}

/*  
* linking n with n2, n2 with n
*
*/
void node_set_double_link(Node *n, Node *n2) {
  if(n == NULL or n2 == NULL) handle_error("trying to link with NULL nodes");
  maybe_realloc_neighbours(n);
  maybe_realloc_neighbours(n2);
  n->neighbours[n->num_neighbours++] = n2;
  n2->neighbours[n2->num_neighbours++] = n;
}

/*
* linking n with n2 at the passed position, that's not the index, if a position is greater than
* the number of neighbours it's index will be num_neighbours
* the node previous linked at the position will be moved to the last index
*/
void node_set_link_at(Node *n, Node *n2, unsigned position) {
  if(n == NULL or n2 == NULL) handle_error("trying to link with NULL nodes");
  if(position < 0) handle_error("invalid neighbour position");
  if(position > n->num_neighbours) node_set_link(n, n2);
  maybe_realloc_neighbours(n);
  n->neighbours[n->num_neighbours++] = n->neighbours[position];
  n->neighbours[position] = n2;
}

/*
* linking n with n2 at the passed position_n, and n2 with n at the passed position_n2
* if position is greater than the number of neighbours the index will be num_neighbours
* the node previous linked at the position will be moved to the last index
*/
void node_set_double_link_at(Node *n, Node *n2, unsigned position_n, unsigned position_n2) {
  if(n == NULL or n2 == NULL) handle_error("trying to link with NULL nodes");
  if(position_n < 0 or position_n2 < 0) handle_error("invalid neighbour position");

  if(position_n > n->num_neighbours) node_set_link(n, n2);
  else{
    maybe_realloc_neighbours(n);
    n->neighbours[n->num_neighbours++] = n->neighbours[position_n];
    n->neighbours[position_n] = n2;
  }
  if(position_n2 > n2->num_neighbours) node_set_link(n2, n);
  else {
    maybe_realloc_neighbours(n2);
    n2->neighbours[n2->num_neighbours++] = n2->neighbours[position_n2];
    n2->neighbours[position_n2] = n;
  }
}

/*
* return the neighbour node at that position
*
*/
Node *node_get_neighbour(Node *n, unsigned neighbour_num) {
  if(n == NULL) handle_error("trying to get neightbour of a NULL address");
  if(neighbour_num < 0 or neighbour_num >= n->num_neighbours) handle_error("trying to access a neighbour out of bounds");

  return n->neighbours[neighbour_num];
}

/*
* swap the position of two neighbours
*
*/
Node *node_swap_neighbour_position(Node *n, unsigned first_position, unsigned second_position) {
  if(n == NULL) handle_error("trying to change postion of a NULL address");
  unsigned len = n->num_neighbours;
  if(first_position < 0 or first_position >= len or second_position < 0 or second_position >= len)
    handle_error("invalid position");
  
  Node *tmp = n->neighbours[first_position];
  n->neighbours[first_position] = n->neighbours[second_position];
  n->neighbours[second_position] = tmp;
}

unsigned node_get_num_neighbours(Node *n) {
  if(n == NULL) handle_error("trying to get number of neighbours of a NULL address");

  return n->num_neighbours;
}

void node_extend_link();