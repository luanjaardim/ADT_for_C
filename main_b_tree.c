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

#include "ADTs/b_tree.h"
// #include "ADTs/b_tree.c"
// #include "ADTs/node.c"

INIT_B_TREE_TYPE(sla, int, float)

int main(int argc, const char **argv) {

  B_Tree *bt = sla_btree_create(2, 5.5, NULL);
  sla_btree_insert(bt, 0, 4.2);
  sla_btree_insert(bt, 1, 6.82);

  float f = 5.5;
  int out = sla_btree_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btree_insert(bt, 3, 9.99999);
  out = sla_btree_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btree_insert(bt, 3, 10.01);
  out = sla_btree_get_value(bt, 1, &f);
  printf("%d %f\n", out, f);

  btree_delete(bt);

  return 0;
}