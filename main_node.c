#include "ADTs/node.h"
// #include "ADTs/node.c"
#define f(a, b) for(int a = 0; a < b; a++)

int main(int argc, const char **argv) {

  int l = 1, l2;
  char sla = 'c', sla2;
  float oi = 5.55, oi2;
  Node *n1, *n2, *n3;
  n1 = node_create(&l, sizeof(l), NULL);
  n2 = node_create(&sla, sizeof(sla), n1);
  n3 = node_create(&oi, sizeof(oi), n2);

  node_get_value(n3, &oi2);
  printf("%f\n", oi2);
  node_get_value(node_get_next(n3), &sla2);
  printf("%c\n", sla2);
  node_get_value(node_get_next(node_get_next(n3)), &l2);
  printf("%d\n", l2);

  node_delete_recursive(n3);
  return 0;
}