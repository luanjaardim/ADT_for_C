#include "ADTs/node.h"
// #include "ADTs/node.c"
#define f(a, b) for(int a = 0; a < b; a++)

int main(int argc, const char **argv) {

  int l = 10, l2;
  char c = 'z', c2;
  float f = 5.5, f2;

  Node *n1, *n2, *n3;

  n1 = node_create(&l, INT_TYPE, NULL, NULL);
  n2 = node_create(&c, CHAR_TYPE, NULL, NULL);
  n3 = node_create(&f, FLOAT_TYPE, NULL, NULL);

  node_set_next(n2, n3); //n2- > n3
  node_set_next(n1, n2); //n1 -> n2 -> n3
  node_set_prev(n2, n1); //n1 <-> n2 -> n3
  node_set_prev(n3, n2); //n1 <-> n2 <-> n3

  node_get_value(n1, &l2);
  node_get_value(node_get_next(n1), &c2);
  node_get_value(node_get_next(node_get_next(n1)), &f2);

  printf("%d %c %f\n", l2, c2, f2);

  node_delete_recursive(n1);

  return 0;
}