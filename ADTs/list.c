#include "list.h"

#define MAX_DATA_SIZE 8
#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct List {
  Node *head, *tail;
  unsigned len;
} List;

/*
* return a pointer to a List instance
*
*/
List *list_create() {
  //ll -> linked list
  List *ll = (List *) malloc(sizeof(List));
  ll->head = node_create_null();
  ll->tail = ll->head;
  ll->len = 0;
}

/*
* delete the List allocated on heap
*
*/
void list_delete(List *ll) {
  if(ll == NULL) handle_error("trying to delete a NULL list");
  node_delete_recursive(ll->head);
  free(ll);
  ll = NULL;
}

/*
* inserts the given value at the end ofthe list
*
*/
void list_append_impl(List *ll, void *to_add, StdDataTypes type) {
  if(ll == NULL) handle_error("trying to append a NULL list");
  if(to_add == NULL) handle_error("trying to append with a NULL address");

  size_t bytes = node_data_size(ll->tail);
  u_int8_t cpy[bytes];
  Node *tmp_node = ll->tail;
  node_set_value(tmp_node, cpy, to_add, type);

  ll->tail = node_create_null();
  node_set_next(tmp_node, ll->tail);
  node_set_prev(ll->tail, tmp_node);

  ll->len++;
}

/*
* removes the last element of the list
* returns -1 if the list is empty, 0 if the pop occured
*
*/
int list_pop_impl(List *ll, void *to_ret, StdDataTypes type) {
  if(ll == NULL) handle_error("trying to pop a NULL list");
  if(to_ret == NULL) handle_error("trying to pop with a NULL address");

  Node *node_to_pop = node_get_prev(ll->tail);
  if(node_to_pop == NULL) return -1; //empty

  char zero = 0;
  node_set_value(node_to_pop, to_ret, (void *) &zero, CHAR_TYPE);
  node_set_next(node_to_pop, NULL);

  node_delete(ll->tail);
  ll->tail = node_to_pop;
  return 0;
}

/*  
* returns the list lenght
*
*/
unsigned list_len(List *ll) {
  if(ll == NULL) handle_error("trying to get len of a NULL address");
  return ll->len;
}

/*
* a type for iterating over lists
*
*/
typedef struct ListIterator {
  Node *node;
} ListIterator;

/*
* returns a pointer to a ListIterator instance
*
*/
ListIterator *iterator_create(List *ll, unsigned index) {
  if(ll == NULL) handle_error("trying to create a iterator from a NULL list");
  if(index >= ll->len) return NULL;

  ListIterator *lli = (ListIterator *) malloc(sizeof(ListIterator));
  if(lli == NULL) handle_error("fail to malloc list iterator");

  Node *node_to_iterator = ll->head;
  while(index--) {
    node_to_iterator = node_get_next(node_to_iterator);
  }
  lli->node = node_to_iterator;
  return lli;
}

/*
* delete the passed ListIterator
*
*/
void iterator_delete(ListIterator *lli) {
  free(lli);
  lli = NULL;
}

/*
* returns the type of the current iterator position on the list
*
*/
StdDataTypes iterator_get_type(ListIterator *lli) {
  if(lli == NULL) handle_error("trying to get type from a NULL iterator");
  return node_get_type(lli->node);
}

/*
* returns the value on the iterator position and advance to next position
*
*/
int iterator_next(ListIterator *lli, void *to_cpy) {
  if(lli == NULL) handle_error("trying advance with next on a NULL iterator");
  if(node_get_next(lli->node) == NULL) { //tail
    free(lli);
    return -1;
  }
  Node *tmp = lli->node;
  node_get_value(tmp, to_cpy);
  lli->node = node_get_next(tmp);
  return 0;
}

/*
* iterate over a list from the start_index till the end of the list, printing every element
*
*/
void iterate_over(List *ll, unsigned start_index) {
  if(ll == NULL) handle_error("trying to iterate over a NULL list");
  if(!ll->len) return;
  if(start_index >= ll->len) return;

  StdDataTypes type;
  ListIterator *lli = iterator_create(ll, start_index);
  char c; short s; int i; float f; double d; long long l; int *p;
  while(1) {
    type = iterator_get_type(lli);

    switch (type)
    {
    case CHAR_TYPE:
      int ret = iterator_next(lli, &c);
      if(ret == -1) {
        printf("\n");
        return;
      }
      printf("%c ", c);
    break;
    case SHORT_TYPE:
      iterator_next(lli, &s);
      printf("%d ", s);
    break;
    case INT_TYPE:
      iterator_next(lli, &i);
      printf("%d ", i);
    break;
    case FLOAT_TYPE:
      iterator_next(lli, &f);
      printf("%f ", f);
    break;
    case DOUBLE_TYPE:
      iterator_next(lli, &d);
      printf("%lf ", d);
    break;
    case LONG_LONG_TYPE:
      iterator_next(lli, &l);
      printf("%lld ", l);
    break;
    case POINTER_TYPE:
      iterator_next(lli, &p);
      printf("%p ", p);
    break;
    default:
      return;
    break;
    }
  }
}