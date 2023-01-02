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

int main(int argc, const char **argv) {

  B_Tree *bt = btree_create();
  
  btree_delete(bt);

  return 0;
}