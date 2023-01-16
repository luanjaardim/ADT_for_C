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

unsigned num = 0;

#include "ADTs/lib.h"
// #include "ADTs/b_tree.c"
// #include "ADTs/node.c"

typedef struct Stru_Key {
  int *vec;
} Stru_Key;

typedef struct Stru_Val {
  char *vec_c;
} Stru_Val;

INIT_BT_MAP_TYPE(sla, int, float)
INIT_BT_MAP_TYPE(name, int, int)
INIT_BT_MAP_TYPE(v, Stru_Key *, Stru_Val *)

Stru_Key *create_key() {
  Stru_Key *key = malloc(sizeof(Stru_Key));
  key->vec = malloc(sizeof(int));
  int random_key = num++;
  memcpy(key->vec, &random_key, sizeof(int));
  return key;
}

Stru_Val *create_val() {
  Stru_Val *val = malloc(sizeof(Stru_Val));
  val->vec_c = malloc(sizeof(char) * 10);
  return val;
}

void my_delete_pair(BT_Pair *pair) {
  Stru_Key *key = v_pair_get_key(pair);
  Stru_Val *value = v_pair_get_value(pair);
  printf("%d\n", *(key->vec));
  free(value->vec_c);
  free(value);
  free(key->vec);
  free(key);
}

// void node_right_rotate(Node *n);
Node *btmap_search_key(BT_Map *bt, void *key, size_t key_size);


int main(int argc, const char **argv) {

  int out;
  BT_Map *bt = sla_btmap_create_with_pair(2, 5.5);
  sla_btmap_insert(bt, 0, 4.2);
  sla_btmap_insert(bt, 1, 6.82);

  float f = 5.5;
  out = sla_btmap_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btmap_insert(bt, 3, 9.99999);
  out = sla_btmap_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btmap_insert(bt, 3, 10.01);
  out = sla_btmap_get_value(bt, 2, &f);
  printf("%d %f\n", out, f);

  printf("%d\n", btmap_get_num_pairs(bt));
  out = sla_btmap_remove(bt, 1, &f);
  printf("%d %f %d\n", out, f, btmap_get_num_pairs(bt));

  btmap_delete(bt);

  BT_Map *bt2 = name_btmap_create_with_pair(100, 100);
  name_btmap_insert(bt2, 200, 200);
  name_btmap_insert(bt2, 150, 150);
  name_btmap_insert(bt2, 300, 300);
  name_btmap_insert(bt2, 50, 50);
  name_btmap_insert(bt2, 0, 0);
  name_btmap_insert(bt2, 75, 75);
  name_btmap_insert(bt2, 99, 99);

  int ret;
  name_btmap_remove(bt2, 100, &ret);
  printf("%d\n", ret);
  name_btmap_remove(bt2, 0, &ret);
  printf("%d\n", ret);
  name_btmap_remove(bt2, 50, &ret);
  printf("%d\n", ret);
  out = name_btmap_remove(bt2, 100, NULL);
  printf("%d %d\n", ret, out); //not found to remove

  btmap_delete(bt2);

  //custom values for key or value, correctly deallocating them
  BT_Map *my_tree = v_btmap_create_with_pair(create_key(), create_val());
  v_btmap_insert(my_tree, create_key(), create_val());
  v_btmap_insert(my_tree, create_key(), create_val());

  //a way to deallocate custom values, it doesn't prevent from insert with the same key, it will only overwrite the 
  //previous key and value, losing them
  btmap_pairs_visit(my_tree, my_delete_pair);
  btmap_delete(my_tree);
  printf("simmmm\n");

  // left rotation on 1
  BT_Map *bt3 = sla_btmap_create_with_pair(1, 1.0);
  sla_btmap_insert(bt3, 2, 8.7);
  sla_btmap_insert(bt3, 3, 6.7);
  sla_btmap_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 3, &f);
  printf("%f\n", f);

  //right rotation on 3
  sla_btmap_insert(bt3, 3, 8.7);
  sla_btmap_insert(bt3, 2, 1.7);
  sla_btmap_insert(bt3, 1, 2.7);
  sla_btmap_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 3, &f);
  printf("%f\n", f);

  //left rotation on 1 and right rotation on 3
  sla_btmap_insert(bt3, 3, 5.48);
  sla_btmap_insert(bt3, 1, 2.34);
  sla_btmap_insert(bt3, 2, 1.32);
  sla_btmap_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 3, &f);
  printf("%f\n", f);

  //right rotation on 3 and left rotation on 1
  sla_btmap_insert(bt3, 1, 10.48);
  sla_btmap_insert(bt3, 3, 8.354);
  sla_btmap_insert(bt3, 2, 99.032);
  sla_btmap_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btmap_remove(bt3, 3, &f);
  printf("%f\n", f);

  btmap_delete(bt3);

  BT_Map *bm = btmap_create();
  int oi = 5, epa = 4;
  btmap_insert(bm, &oi, sizeof(int), &epa, sizeof(int));
  btmap_remove(bm, &oi, sizeof(int), NULL, sizeof(int));
  printf("opa meu mano\n");
  btmap_delete(bm);

  return 0;
}