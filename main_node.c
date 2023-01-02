#include "ADTs/node.h"
// #include "ADTs/node.c"
#define f(a, b) for(int a = 0; a < b; a++)

typedef struct Sla {
  int *opa;
  int oi;
} Sla;

INIT_NODE_TYPE(i, int)
INIT_NODE_TYPE(mine, Sla *)

Sla *create() {
  Sla *sla = (Sla *) malloc(sizeof(Sla *));
  sla->opa = malloc(sizeof(int *));
  return sla;
}

void delete(Node *n) {
  Sla *sla = mine_node_get_value(n);
  free(sla->opa);
  free(sla);
}

int main(int argc, const char **argv) {

  Node *n = i_node_create(1);

  node_set_link(n, i_node_create(4));
  node_set_link(n, i_node_create(5));
  node_set_link_at(n, i_node_create(2), 0);
  node_set_double_link_at(n, i_node_create(3), 1, 50);

  int tmp;
  f(i, node_get_num_neighbours(n)) {
    tmp  = i_node_get_value(node_get_neighbour(n, i));
    printf("%d ", tmp);
  }
  printf("\n");

  node_swap_neighbour_position(n, 0, node_get_num_neighbours(n) - 1);
  f(i, node_get_num_neighbours(n)) {
    tmp  = i_node_get_value(node_get_neighbour(n, i));
    printf("%d ", tmp);
  }
  printf("\n");

  unsigned a = node_delete_recursive(n, NULL);
  printf("%d\n", a);


  Sla *opa = create();
  Node *node = mine_node_create(opa);
  node_delete(node, delete); //custom delete, for node elements that need own deallocation

  return 0;
}