#include "list.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct List {
  Node *head, *tail;
  unsigned len;
} List;

List *list_create() {
  //ll -> linked list
  List *ll = (List *) malloc(sizeof(List));
  ll->head = node_create_null();
  ll->tail = ll->head;
  ll->len = 0;
}

void list_delete(List *ll) {
  if(ll == NULL) handle_error("trying to delete a NULL list");
  node_delete_recursive(ll->head);
  ll->head = NULL;
  free(ll);
  ll = NULL;
}

void list_append_impl(List *ll, void *to_add, size_t data_size) {
  if(ll == NULL) handle_error("trying to append a NULL list");
  if(to_add == NULL) handle_error("trying to append with a NULL address");

  size_t bytes = node_data_size(ll->tail);
  u_int8_t cpy[bytes];
  Node *tmp_node = ll->tail;
  node_set_value(tmp_node, cpy, to_add, data_size);

  ll->tail = node_create_null();
  node_set_next(tmp_node, ll->tail);

  ll->len++;
}

unsigned list_len(List *ll) {
  if(ll == NULL) handle_error("trying to get len of a NULL address");
  return ll->len;
}

typedef struct ListIterator {
  Node *node;
} ListIterator;

ListIterator *iterator_create(List *ll) {
  if(ll == NULL) handle_error("trying to create a iterator from a NULL list");
  
  ListIterator *lli = (ListIterator *) malloc(sizeof(ListIterator));
  if(lli == NULL) handle_error("fail to malloc list iterator");
  lli->node = ll->head;
  return lli;
}

void iterator_delete(ListIterator *lli) {
  free(lli);
  lli = NULL;
}

int iterator_next(ListIterator *lli, void *to_cpy) {
  if(lli == NULL) handle_error("trying advance with next on a NULL iterator");
  if(node_get_next(lli->node) == NULL) {
    free(lli);
    return -1;
  }
  Node *tmp = lli->node;
  node_get_value(tmp, to_cpy);
  lli->node = node_get_next(tmp);
  return 0;
}