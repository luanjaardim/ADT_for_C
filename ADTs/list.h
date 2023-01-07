#ifndef LIST_LIB_DEF
#define LIST_LIB_DEF

#include "gnode.h"

typedef struct List List;
typedef struct ListIterator ListIterator;

List *list_create();
void list_delete(List *ll);
void list_insert_impl(List * ll, void *to_add, StdDataTypes type, unsigned position);
void list_append_impl(List *ll, void *to_add, StdDataTypes type);
void list_remove_impl(List *ll, void *to_ret, unsigned position);
int list_pop_impl(List *ll, void *to_ret);
unsigned list_len(List *ll);
ListIterator *iterator_create(List *ll, unsigned index);
void iterator_delete(ListIterator *lli);
StdDataTypes iterator_get_type(ListIterator *lli);
int iterator_next(ListIterator *lli, void *to_cpy);
void iterate_over(List *ll, unsigned start_index);

#endif /* LIST_LIB_DEF */