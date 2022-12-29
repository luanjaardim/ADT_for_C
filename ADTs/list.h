#ifndef LIST_LIB_DEF
#define LIST_LIB_DEF

#include "node.h"

typedef struct List List;
typedef struct ListIterator ListIterator;

List *list_create();
void list_delete(List *ll);
void list_append_impl(List *ll, void *to_add, size_t data_size, StdDataTypes type);
unsigned list_len(List *ll);
ListIterator *iterator_create(List *ll);
int iterator_next(ListIterator *lli, void *to_cpy);
void iterate_over(List *ll);

#list_append(ll, val, type) list_append_impl(ll, (void *) &(val), sizeof(val), type)

#endif /* LIST_LIB_DEF */