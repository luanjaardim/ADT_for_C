#include "ADTs/node.h"
// #include "ADTs/node.c"
#define f(a, b) for(int a = 0; a < b; a++)

int main(int argc, const char **argv) {

  // int l = 1, l2;
  // char sla = 'c', sla2;
  // float oi = 5.55, oi2;
  // Node *n1, *n2, *n3;
  // n1 = node_create(&l, sizeof(l), INT_TYPE, NULL);
  // n2 = node_create(&sla, sizeof(sla), CHAR_TYPE, n1);
  // n3 = node_create(&oi, sizeof(oi), FLOAT_TYPE, n2);

  // Node *tmp = n3;
  // while(tmp != NULL) {

  //   if(node_get_type(tmp) == INT_TYPE) {
  //     node_get_value(tmp, &l2);
  //     printf("%d\n", l2);
  //   }
  //   else if(node_get_type(tmp) == FLOAT_TYPE) {
  //     node_get_value(tmp, &oi2);
  //     printf("%f\n", oi2);
  //   }
  //   else if(node_get_type(tmp) == CHAR_TYPE) {
  //     node_get_value(tmp, &sla2);
  //     printf("%c\n", sla2);
  //   }
  //   tmp = node_get_next(tmp);
  // }

  // node_delete_recursive(n3);

  void *data = malloc(8);
  Node *n = node_create_null();
  char opa[node_data_size(n)];
  node_set_value(n, opa, data, 8, LONG_LONG_TYPE);
  node_delete(n);
  free(data);
  return 0;
}