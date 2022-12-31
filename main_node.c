#include "ADTs/node.h"
// #include "ADTs/node.c"
#define f(a, b) for(int a = 0; a < b; a++)
INIT_NODE_TYPE(i, int)

int main(int argc, const char **argv) {

  Node *n = i_node_create(1);

  node_set_link(n, i_node_create(2));
  node_set_link(n, i_node_create(10));
  node_set_double_link(n, i_node_create(3));

  int sla = i_node_set_value(n, 20);

  unsigned a = node_delete_recursive(n);
  printf("%d %d\n", a, sla);
  return 0;
}