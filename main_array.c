#include "ADTs/lib.h"
// #include "ADTs/array.c"
#include <time.h>
#define f(a, b) for(int a=0; a<b; a++)


typedef struct MyType {
  int *sla;
} MyType;

INIT_ARRAY_TYPE(v, Array *)
INIT_ARRAY_TYPE(i, int)
INIT_ARRAY_TYPE(c, char)
INIT_ARRAY_TYPE(a, MyType *)

MyType *create() {
  MyType *mt = (MyType *) malloc(sizeof(MyType));
  mt->sla = (int *) malloc(sizeof(int));
  return mt;
}

void delete_data(Array *ar) {
  unsigned len = array_len(ar);
  f(i, len) {
    MyType *mt = a_array_get(ar, i);
    free(mt->sla);
    free(mt);
  }
  array_delete(ar);
}

int my_own_cmp(Array *ar, unsigned ind1, unsigned ind2) {
  Array *ar1 = v_array_get(ar, ind1), *ar2 = v_array_get(ar, ind2);

  return (int) (array_len(ar1) - array_len(ar2));
}

int main(int argc, const char **argv) {

    Array *ar = v_array_create();
  f(i, 5) v_array_append(ar, i_array_create());


  v_array_delete_data(ar, array_delete);
  array_delete(ar);


  Array *ar2 = v_array_create();
  f(i, 5) v_array_append(ar2, a_array_create());
  f(i, 4) a_array_append(v_array_get(ar2, 0), create());

  unsigned len = array_len(v_array_get(ar2, 0));
  printf("%d\n", len);

  Array *popped = v_array_pop(ar2);

  v_array_delete_data(ar2, delete_data);
  array_delete(ar2);
  array_delete(popped);

  // MATRIX WITH CUSTOM SORTING
  srand(time(NULL));
  Array *ar3 = v_array_create();
  int qtd = (rand() % 9) + 1;
  f(i, qtd) {
    v_array_append(ar3, i_array_create());
    int qtd_elements = (rand() % 19) + 1;
    f(j, qtd_elements) {
      i_array_append(v_array_get(ar3, i), rand() % 1000);
    }
    array_sort(v_array_get(ar3, i), NULL); //passing NULL for standard comparing function
  }
  array_sort(ar3, my_own_cmp); //custom compare function for sorting

  len = array_len(ar3);
  f(i, len) {
    array_print_elements(v_array_get(ar3, i), "%d ", int);
    array_delete(v_array_get(ar3, i));
  }
  array_delete(ar3);

  return 0;
}
