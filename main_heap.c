#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#define pt pthread_
#define f(a, b) for(int a = 0; a < b; a++)
#define and &&
#define or ||

#include "ADTs/lib.h"
// #include "ADTs/heap.c"
// #include "ADTs/array.c"

INIT_HEAP_TYPE(i, int)
INIT_ARRAY_TYPE(i, int)

int main(int argc, const char **argv) {

  Heap *h = i_heap_create(1, NULL);
  i_heap_put(h, 1);
  i_heap_put(h, 2);
  i_heap_put(h, 1);
  i_heap_put(h, 3);
  i_heap_put(h, 9);
  i_heap_put(h, 5);
  i_heap_put(h, 1);
  debug(h);
  int sla;
  while (i_heap_pop(h, &sla) != -1)
  {
    printf("%d\n", sla);
  }

  int opa[5] = {1, 2, 3, 4, 5};
  int *opa2 = (int *) malloc(sizeof(int) * 5);
  f(i, 5) opa2[i] = i;
  Array *ar = i_array_create();
  i_array_extend(ar, opa, sizeof(opa)/sizeof(opa[0]));
  i_array_extend(ar, opa2, 5);

  heap_from_array(h, &ar);
  array_delete(ar);

  debug(h);
  free(opa2);

  heap_change_status(h);
  debug(h);
  i_heap_put(h, 0);
  debug(h);
  
  heap_delete(&h); 

  return 0;
}