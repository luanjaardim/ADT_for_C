#include "ADTs/array.h"
// #include "ADTs/array.c"
#include <time.h>
#define f(a, b) for(int a=0; a<b; a++)

ARRAY_TYPE_INIT(v, Array *)
ARRAY_TYPE_INIT(i, int)
ARRAY_TYPE_INIT(c, char)

int my_own_cmp(Array *ar, unsigned ind1, unsigned ind2) {
  Array **elements = (Array **) array_data_pointer(ar);
  Array *ar1 = elements[ind1], *ar2 = elements[ind2];

  return (int) (array_len(ar1) - array_len(ar2));
}

int main(int argc, const char **argv) {

  //MATRIX WITH CUSTOM SORTING
  // srand(time(NULL));
  // Array *ar = v_array_create();
  // int qtd = (rand() % 9) + 1;
  // f(i, qtd) {
  //   v_array_append(ar, i_array_create());
  //   int qtd_elements = (rand() % 19) + 1;
  //   f(j, qtd_elements) {
  //     i_array_append(v_array_get(ar, i), rand() % 1000);
  //   }
  //   array_sort(v_array_get(ar, i), array_std_cmp);
  // }
  // array_sort(ar, my_own_cmp); //custom compare function for sorting

  // unsigned len = array_len(ar);
  // f(i, len) {
  //   array_print_elements(v_array_get(ar, i), "%d ", int);
  //   array_delete(v_array_get(ar, i));
  // }

  // array_delete(ar);


  //USING DELETE ALL
  // Array *ar = i_array_create();
  // Array *ar2 = i_array_create();
  // Array *ar3 = i_array_create();

  // Array *ai_mds = v_array_create();
  // v_array_append(ai_mds, ar);
  // v_array_append(ai_mds, ar2);
  // v_array_append(ai_mds, ar3);
  // array_delete(v_array_get(ai_mds, 0));
  // // array_delete(ar);
  // // array_delete(ar2);
  // // array_delete(ar3);
  // array_delete_all();



  // Array *ar = i_array_create();

  // int sla[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  // i_array_extend(ar, sla, sizeof(sla)/sizeof(sla[0]));

  // array_print_elements(ar, "%d ", int);

  // array_delete(ar);

  // ar = c_array_create();

  // char *palavra = "opa meu maninho";
  // c_array_extend(ar, palavra, strlen(palavra)); 

  // array_print_elements(ar, "%c", char);

  // array_delete(ar);

  

  return 0;
}
