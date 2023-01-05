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

#include "ADTs/b_tree.h"
#include "ADTs/node.h"
// #include "ADTs/b_tree.c"
// #include "ADTs/node.c"

typedef struct Stru_Key {
  int *vec;
} Stru_Key;

typedef struct Stru_Val {
  char *vec_c;
} Stru_Val;

int my_key_cmp_func(StdPairKV *pair1, StdPairKV *pair2, size_t key_size) {
  Stru_Key *k1, *k2;
  pair_get_key(pair1, &k1, sizeof(Stru_Key *));
  pair_get_key(pair2, &k2, sizeof(Stru_Key *));
  if(*(k1->vec) == *(k2->vec)) return 0;
  else if(*(k1->vec) > *(k2->vec)) return 1;
  else return 0;
}

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

void my_delete_pair(StdPairKV *pair) {
  Stru_Key *key;
  Stru_Val *value;
  pair_get_key_and_value(pair, &key, &value, sizeof(Stru_Key *), sizeof(Stru_Val *));
  printf("%d\n", *(key->vec));
  free(value->vec_c);
  free(value);
  free(key->vec);
  free(key);
}

INIT_B_TREE_TYPE(sla, int, float)
INIT_B_TREE_TYPE(name, int, int)
INIT_B_TREE_TYPE(v, Stru_Key *, Stru_Val *)

// void node_right_rotate(Node *n);
Node *btree_search_key(B_Tree *bt, void *key, size_t key_size);


int main(int argc, const char **argv) {

  int out;
  B_Tree *bt = sla_btree_create(2, 5.5, NULL);
  sla_btree_insert(bt, 0, 4.2);
  sla_btree_insert(bt, 1, 6.82);

  float f = 5.5;
  out = sla_btree_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btree_insert(bt, 3, 9.99999);
  out = sla_btree_get_value(bt, 3, &f);
  printf("%d %f\n", out, f);
  sla_btree_insert(bt, 3, 10.01);
  out = sla_btree_get_value(bt, 2, &f);
  printf("%d %f\n", out, f);

  printf("%d\n", get_num_pairs(bt));
  out = sla_btree_remove(bt, 1, &f);
  printf("%d %f %d\n", out, f, get_num_pairs(bt));

  btree_delete(bt);

  B_Tree *bt2 = name_btree_create(100, 100, NULL);
  name_btree_insert(bt2, 200, 200);
  name_btree_insert(bt2, 150, 150);
  name_btree_insert(bt2, 300, 300);
  name_btree_insert(bt2, 50, 50);
  name_btree_insert(bt2, 0, 0);
  name_btree_insert(bt2, 75, 75);
  name_btree_insert(bt2, 99, 99);

  int ret;
  name_btree_remove(bt2, 100, &ret);
  printf("%d\n", ret);
  name_btree_remove(bt2, 0, &ret);
  printf("%d\n", ret);
  name_btree_remove(bt2, 50, &ret);
  printf("%d\n", ret);
  out = name_btree_remove(bt2, 100, &ret);
  printf("%d %d\n", ret, out); //not found to remove

  btree_delete(bt2);

  //custom values for key or value, correctly deallocating them
  B_Tree *my_tree = v_btree_create(create_key(), create_val(), my_key_cmp_func);
  v_btree_insert(my_tree, create_key(), create_val());
  v_btree_insert(my_tree, create_key(), create_val());

  //a way to deallocate custom values, it doesn't prevent from insert with the same key, it will only overwrite the 
  //previous key and value, losing them
  btree_pairs_visit(my_tree, my_delete_pair);
  btree_delete(my_tree);
  printf("simmmm\n");

  // left rotation on 1
  B_Tree *bt3 = sla_btree_create(1, 1.0, NULL);
  sla_btree_insert(bt3, 2, 8.7);
  sla_btree_insert(bt3, 3, 6.7);
  sla_btree_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 3, &f);
  printf("%f\n", f);

  //right rotation on 3
  sla_btree_insert(bt3, 3, 8.7);
  sla_btree_insert(bt3, 2, 1.7);
  sla_btree_insert(bt3, 1, 2.7);
  sla_btree_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 3, &f);
  printf("%f\n", f);

  //left rotation on 1 and right rotation on 3
  sla_btree_insert(bt3, 3, 5.48);
  sla_btree_insert(bt3, 1, 2.34);
  sla_btree_insert(bt3, 2, 1.32);
  sla_btree_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 3, &f);
  printf("%f\n", f);

  //right rotation on 3 and left rotation on 1
  sla_btree_insert(bt3, 1, 10.48);
  sla_btree_insert(bt3, 3, 8.354);
  sla_btree_insert(bt3, 2, 99.032);
  sla_btree_remove(bt3, 1, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 2, &f);
  printf("%f\n", f);
  sla_btree_remove(bt3, 3, &f);
  printf("%f\n", f);

  btree_delete(bt3);

  return 0;
}