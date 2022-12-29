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

#include "ADTs/list.h"
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

  list_append_impl(ll, &opa, sizeof(opa), CHAR_TYPE);
  list_append_impl(ll, &sla, sizeof(sla), CHAR_TYPE);
  list_append_impl(ll, &one, sizeof(one), INT_TYPE);
  list_append_impl(ll, &two, sizeof(two), INT_TYPE);
  list_append_impl(ll, &gul, sizeof(gul), FLOAT_TYPE);
  list_append_impl(ll, &foo, sizeof(foo), FLOAT_TYPE);
  list_append_impl(ll, &test, sizeof(test), SHORT_TYPE);
  list_append_impl(ll, &test2, sizeof(test2), SHORT_TYPE);
  list_append_impl(ll, &d1, sizeof(d1), DOUBLE_TYPE);
  list_append_impl(ll, &d2, sizeof(d2), DOUBLE_TYPE);
  list_append_impl(ll, &longol, sizeof(longol), LONG_LONG_TYPE);
  list_append_impl(ll, pnt1, sizeof(pnt1), POINTER_TYPE);
  list_append_impl(ll, pnt2, sizeof(pnt2), POINTER_TYPE);
  iterate_over(ll);

  list_delete(ll);

  return 0;
}