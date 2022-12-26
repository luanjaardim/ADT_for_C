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

void test(List *ll) {
  int cpy;
  Node *tmp = ll->head;
  while(tmp != ll->tail) {
    node_get_value(tmp, &cpy);
    printf("%d ", cpy);
    tmp = node_get_next(tmp);
  }
  printf("\n");
}