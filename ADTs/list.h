#ifndef LIST_LIB_DEF
#define LIST_LIB_DEF

#include "node.h"

typedef struct List List;
typedef struct ListIterator ListIterator;

List *list_create();
void list_delete(List *ll);
void list_append_impl(List *ll, void *to_add, StdDataTypes type);
int list_pop_impl(List *ll, void *to_ret, StdDataTypes type);
unsigned list_len(List *ll);
ListIterator *iterator_create(List *ll, unsigned index);
int iterator_next(ListIterator *lli, void *to_cpy);
void iterate_over(List *ll, unsigned start_index);

#endif /* LIST_LIB_DEF */