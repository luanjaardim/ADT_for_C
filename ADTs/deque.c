#include "node.h"
#include "deque.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct Deque {
  Node *first, *last;
  unsigned len;
  void (*delete_data)(void *);
} Deque;

typedef enum NodePositions {
  PREV,
  NEXT,
} NodePositions;

/*
* return a Deque pointer to the data allocated on heap
* delete_data is a custom function for deleting the data stored in deque
* if the data doesn't need heap deallocation just pass NULL
*/
Deque *deque_create(void (*delete_data)(void *)) {
  Deque *new_deque = (Deque *) malloc(sizeof(Deque));
  if(new_deque == NULL) handle_error("fail to malloc deque");

  new_deque->last = NULL;
  new_deque->first = NULL;
  new_deque->delete_data = delete_data;
  new_deque->len = 0;

  return new_deque;
}

/*
* delete the passed Deque freeing all memmory related to it
*
*/
void deque_delete(Deque *dq) {
  if(dq == NULL) handle_error("trying to delete with a NULL address");
  if(dq->first != NULL)
    node_delete_recursive(dq->first, dq->delete_data);
  free(dq);
  dq = NULL;
}

/*
* return true if the Deque is empty, else return false 
*
*/
bool deque_is_empty(Deque *dq) {
  if(dq == NULL) handle_error("trying to see if a NULL address is empty");

  if(dq->len) return false;
  return true;
}

/* 
* create a node with two links, to the next and to previous node, at the begin they are NULL
*
*/
Node *deque_node_create(void *value, size_t data_size) {
  Node *n = node_create(value, data_size);
  node_set_link(n, NULL); //previous node
  node_set_link(n, NULL); //next node
  return n;
}

/* 
* push a element on a empty Deque
*
*/
void push_to_empty(Deque *dq, void *to_add, size_t data_size) {
  Node *n = deque_node_create(to_add, data_size);
  dq->first = n;
  dq->last = n;
  dq->len++;
}

/*
* push a element at the front of the Deque
*
*/
void deque_push_front(Deque *dq, void *to_add, size_t data_size) {
  if(deque_is_empty(dq)) {
    push_to_empty(dq, to_add, data_size);
    return;
  }
  Node *n = deque_node_create(to_add, data_size);
  node_set_double_link_at(dq->first, n, PREV, NEXT);
  dq->first = n;
  dq->len++;
}

/*
* push a element at the back of the Deque
*
*/

void deque_push_back(Deque *dq, void *to_add, size_t data_size) {
  if(deque_is_empty(dq)) {
    push_to_empty(dq, to_add, data_size);
    return;
  }
  Node *n = deque_node_create(to_add, data_size);
  node_set_double_link_at(dq->last, n, NEXT, PREV);
  dq->last = n;
  dq->len++;
}

/*
* remove a element at the front of the Deque
* return 0 if the pop succed, and -1 if not(empty)
*/

int deque_pop_front(Deque *dq, void *to_ret, size_t data_size) {
  if(deque_is_empty(dq)) return -1;
  Node *new_first = node_get_neighbour(dq->first, NEXT);
  if(to_ret != NULL) node_get_value(dq->first, to_ret, data_size);
  node_delete(dq->first, dq->delete_data);

  if(new_first != NULL) node_remove_link_at(new_first, PREV);
  else dq->last = NULL;
  dq->first = new_first;
  dq->len--;
  return 0;
}

/*
* remove a element at the back of the Deque
* return 0 if the pop succed, and -1 if not(empty)
*/
int deque_pop_back(Deque *dq, void *to_ret, size_t data_size) {
  if(deque_is_empty(dq)) return -1;

  Node *new_last = node_get_neighbour(dq->last, PREV);
  if(to_ret != NULL) node_get_value(dq->last, to_ret, data_size);
  node_delete(dq->last, dq->delete_data);

  if(new_last != NULL) node_remove_link_at(new_last, NEXT);
  else dq->first = NULL; //the deque is empty
  dq->last = new_last;
  dq->len--;
  return 0;
}

/*
* return the lenght of the Deque
*
*/
unsigned deque_len(Deque *dq) {
  if(dq == NULL) handle_error("trying to get the len of a NULL address");
  return dq->len;
}

void deque_extend_with(Deque *dq, void *begin, size_t data_size, unsigned len) {
  for(int tmp = 0; tmp < len; tmp ++) {
    deque_push_front(dq, begin + (tmp * data_size), data_size);
  }
}