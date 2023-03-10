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
// #include "ADTs/list.c"
// #include "ADTs/node.c"

int main(int argc, const char **argv) {

  // List *ll = list_create();

  // list_append(ll, 1, int);
  // list_append(ll, 'c', char);
  // list_append(ll, 5.5, float);

  // list_delete(ll);

  // List *ll = list_create();

  // int l = 1;
  // list_append_impl(ll, &l, sizeof(l), INT_TYPE);
  // l++;
  // list_append_impl(ll, &l, sizeof(l), INT_TYPE);
  // l++;
  // list_append_impl(ll, &l, sizeof(l), INT_TYPE);
  // l++;
  // list_append_impl(ll, &l, sizeof(l), INT_TYPE);

  // ListIterator *lli =  iterator_create(ll);

  // int val;
  // while(iterator_next(lli, &val) != -1) {
  //   printf("%d\n", val);
  // }

  // list_delete(ll);

  List *ll = list_create();

  char opa = 'c', sla = '!';
  int one = 1, two = 10;
  float gul = 0.1, foo = 1.10;
  short test = 15, test2 = -19;
  double d1 = 5.55, d2 = -3.21;
  long long longol = 1e9;
  int *pnt1 = &one;
  int **pnt2 = &pnt1;

  list_append_impl(ll, &opa, CHAR_TYPE);
  list_append_impl(ll, &sla, CHAR_TYPE);
  list_append_impl(ll, &one, INT_TYPE);
  list_append_impl(ll, &two, INT_TYPE);
  list_append_impl(ll, &gul, FLOAT_TYPE);
  list_append_impl(ll, &foo, FLOAT_TYPE);
  list_append_impl(ll, &test, SHORT_TYPE);
  list_append_impl(ll, &test2, SHORT_TYPE);
  list_append_impl(ll, &d1, DOUBLE_TYPE);
  list_append_impl(ll, &d2, DOUBLE_TYPE);
  list_append_impl(ll, &longol, LONG_LONG_TYPE);
  list_append_impl(ll, pnt1, POINTER_TYPE);
  list_append_impl(ll, pnt2, POINTER_TYPE);
  iterate_over(ll, 0);

  void *pnt, *another_name;
  list_pop_impl(ll, &pnt);
  list_pop_impl(ll, &another_name);
  longol = 0;
  list_pop_impl(ll, &longol);
  iterate_over(ll, 0);
  iterate_over(ll, 5);

  int a = 55;
  list_insert_impl(ll, &a, INT_TYPE, 5);
  iterate_over(ll, 4);

  a = 9; char c = 0; double f = 5;
  list_remove_impl(ll, &a, 5);
  list_remove_impl(ll, &c, 0);
  list_remove_impl(ll, &f, list_len(ll) - 1);
  printf("%d %c %f\n", a, c, f);
  
  iterate_over(ll, 0);

  //removing and descarting the copy
  list_pop_impl(ll, NULL); 
  list_remove_impl(ll, NULL, 0);

  iterate_over(ll, 0);

  list_delete(ll);

  return 0;
}