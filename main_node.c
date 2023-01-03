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

  node_change_neighbour_position(n, 0, node_get_num_neighbours(n) - 1);
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

  Node *test = i_node_create(1);
  node_set_link(test, NULL);
  node_set_link(test, NULL);
  node_set_link(test, NULL);
  node_set_link_at(test, i_node_create(2), 0);
  Node *n_test = node_get_neighbour(test, 0);
  int sla25 = i_node_get_value(n_test);
  printf("%d %d\n", node_get_num_neighbours(test), sla25);

  node_change_neighbour_position(test, 0, 2);
  Node *n_test2 = node_get_neighbour(test, 2);
  int sla26 = i_node_get_value(n_test2);
  printf("%d\n", sla26);

  Node *test2 = i_node_create(5);
  node_set_double_link_at(test, test2, 1, 50);
  printf("%d %d\n", node_get_num_neighbours(test), node_get_num_neighbours(test2));

  node_delete_recursive(test, NULL);

  Node *my_node = i_node_create(1);
  node_set_double_link_at(my_node, i_node_create(2), 0, 0);
  Node *my_other_node = i_node_create(3);
  node_set_double_link_at(my_other_node, i_node_create(4), 0, 0);
  
  node_swap_neighbours(my_node, my_other_node, 0, 0);
  Node *my_node_n = node_get_neighbour(my_node, 0);
  Node *my_other_node_n = node_get_neighbour(my_other_node, 0);

  int ret1 = i_node_get_value(my_node_n);
  int ret2 = i_node_get_value(my_other_node_n);
  printf("%d %d\n", ret1, ret2);
  ret1 = i_node_get_value(node_get_neighbour(my_node_n, 0));
  ret2 = i_node_get_value(node_get_neighbour(my_other_node_n, 0));
  printf("%d %d\n", ret1, ret2);

  node_set_link(my_node, my_other_node);

  node_delete_recursive(my_node, NULL);

  return 0;
}