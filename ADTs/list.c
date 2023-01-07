#include "list.h"

#define MAX_DATA_SIZE 8
#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct List {
  GNode *head, *tail;
  unsigned len;
} List;

/*
* return a pointer to a List instance
*
*/
List *list_create() {
  //ll -> linked list
  List *ll = (List *) malloc(sizeof(List));
  ll->head = gnode_create_null();
  ll->tail = ll->head;
  ll->len = 0;
  return ll;
}

/*
* delete the List allocated on heap
*
*/
void list_delete(List *ll) {
  if(ll == NULL) handle_error("trying to delete a NULL list");
  gnode_delete_recursive(ll->head);
  free(ll);
  ll = NULL;
}

/*
* walk trough the list to get the node at some position
*
*/
GNode *get_gnode_at_position(List *ll, unsigned index) {
  if(ll == NULL) handle_error("trying to get node at position of a NULL list");
  if(index < 0 or index >= ll->len) handle_error("get node at a position out of bounds");

  GNode *tmp = ll->head;
  while(index--) {
    tmp = gnode_get_next(tmp);
  }
  return tmp;
}

void list_insert_impl(List * ll, void *to_add, StdDataTypes type, unsigned position) {
  if(ll == NULL) handle_error("trying to append a NULL list");
  if(to_add == NULL) handle_error("trying to append with a NULL address");
  if(position < 0 or position >= ll->len) { list_delete(ll); handle_error("position out of bound to insert"); }

  GNode *new_node, *gnode_at_position, *prev_gnode_at_position;

  gnode_at_position = get_gnode_at_position(ll, position);
  new_node = gnode_create(to_add, type, NULL, NULL);

  prev_gnode_at_position = gnode_set_prev(gnode_at_position, new_node);
  gnode_set_next(new_node, 
                gnode_set_next(prev_gnode_at_position, new_node) //returns gnode_at_position
                ); 
  gnode_set_prev(new_node, prev_gnode_at_position);
  
  ll->len++;
}

/*
* inserts the given value at the end ofthe list
*
*/
void list_append_impl(List *ll, void *to_add, StdDataTypes type) {
  if(ll == NULL) handle_error("trying to append a NULL list");
  if(to_add == NULL) handle_error("trying to append with a NULL address");

  size_t bytes = gnode_data_size(ll->tail);
  u_int8_t cpy[bytes];
  GNode *tmp_node = ll->tail;
  gnode_set_value(tmp_node, cpy, to_add, type);

  ll->tail = gnode_create_null();
  gnode_set_next(tmp_node, ll->tail);
  gnode_set_prev(ll->tail, tmp_node);

  ll->len++;
}

/*
* removes the element at position and return it's value on to_ret
* 
*/
void list_remove_impl(List *ll, void *to_ret, unsigned position) {
  if(ll == NULL) handle_error("trying to remove a NULL list");
  if(to_ret == NULL) handle_error("trying to remove with a NULL address");
  if(position < 0 or position >= ll->len) { list_delete(ll); handle_error("position out of bound to remove"); }

  GNode *n = get_gnode_at_position(ll, position), *prev_n, *next_n;
  gnode_get_value(n, to_ret);

  next_n = gnode_get_next(n);
  prev_n = gnode_get_prev(n);
  gnode_set_prev(next_n, prev_n);

  if(prev_n == NULL) //n was the head
    ll->head = next_n;
  else 
    gnode_set_next(prev_n, next_n);
  
  gnode_delete(n);
  ll->len--;
}

/*
* removes the last element of the list and return the value with to_ret
* returns -1 if the list is empty, 0 if the pop occured
*
*/
int list_pop_impl(List *ll, void *to_ret) {
  if(ll == NULL) handle_error("trying to pop a NULL list");
  if(to_ret == NULL) handle_error("trying to pop with a NULL address");

  GNode *gnode_to_pop = gnode_get_prev(ll->tail);
  if(gnode_to_pop == NULL) return -1; //empty

  char zero = 0;
  gnode_set_value(gnode_to_pop, to_ret, (void *) &zero, CHAR_TYPE);
  gnode_set_next(gnode_to_pop, NULL);

  gnode_delete(ll->tail);
  ll->tail = gnode_to_pop;

  ll->len--;
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
  GNode *node;
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

  GNode *gnode_to_iterator = get_gnode_at_position(ll, index);
  lli->node = gnode_to_iterator;
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
  return gnode_get_type(lli->node);
}

/*
* returns the value on the iterator position and advance to next position
*
*/
int iterator_next(ListIterator *lli, void *to_cpy) {
  if(lli == NULL) handle_error("trying advance with next on a NULL iterator");
  if(gnode_get_next(lli->node) == NULL) { //tail
    free(lli);
    return -1;
  }
  GNode *tmp = lli->node;
  gnode_get_value(tmp, to_cpy);
  lli->node = gnode_get_next(tmp);
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