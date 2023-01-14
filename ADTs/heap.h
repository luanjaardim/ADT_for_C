#ifndef HEAP_LIB_DEF
#define HEAP_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct Heap Heap;
typedef struct Array Array;

Heap *heap_create(size_t data_size, bool heap_status, int (*cmp)(void *, void *, size_t));
void heap_delete(Heap *h, void (*delete_data)(void *));
bool heap_is_empty(Heap *h);
int heap_get_top(Heap *h, void *to_cpy);
void heap_put(Heap *h, void *to_add);
int heap_pop(Heap *h, void *to_cpy);
void heap_from_array(Heap *h, Array **ar);
void heap_change_status(Heap *h);
void debug(Heap *h);

#ifndef INIT_HEAP_TYPE
#define INIT_HEAP_TYPE(name, type) \
  Heap *name##_heap_create(bool heap_status, int (*cmp)(void *, void *, size_t)) { \
    return heap_create(sizeof(type), heap_status, cmp); \
  } \
  int name##_heap_get_top(Heap *h, type *val) { \
    return heap_get_top(h, (void *) val); \
  } \
  void name##_heap_put(Heap *h, type val) { \
    heap_put(h, (void *) &val); \
  } \
  int name##_heap_pop(Heap *h, type *val) { \
    return heap_pop(h, (void *) val); \
  }
#endif /* INIT_HEAP_TYPE */

#endif /* HEAP_LIB_DEF */