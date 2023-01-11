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

Heap *heap_create(size_t data_size, bool heap_status, int (*cmp)(void *, void *, size_t));
void heap_delete(Heap **h);


#endif /* HEAP_LIB_DEF */