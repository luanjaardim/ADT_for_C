#ifndef LIST_LIB_DEF
#define LIST_LIB_DEF

#include "node.h"

typedef struct List List;

List *list_create();
void list_delete(List *ll);
void list_append_impl(List *ll, void *to_add, size_t data_size);
void test(List *ll);

#endif /* LIST_LIB_DEF */