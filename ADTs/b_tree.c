#include "b_tree.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct B_Tree {
  Node *root;
  unsigned vertices;
  int (*cmp)(StdPairKV *, StdPairKV *, size_t);
} B_Tree;

typedef struct StdPairKV {
  void *key, *value;
} StdPairKV;

typedef enum LinkedNodes {
  PARENT,
  LEFT,
  RIGHT,
} LinkedNodes;

Node *btree_node_create(void *key, size_t key_size, void *value, size_t value_size) {
  StdPairKV *node_pair = (StdPairKV *) malloc(sizeof(StdPairKV));
  if(node_pair == NULL) handle_error("fail to malloc root pair");
  node_pair->key = malloc(key_size);
  if(node_pair->key == NULL) handle_error("fail to malloc root key's");
  memcpy(node_pair->key, key, key_size);
  node_pair->value = malloc(value_size);
  if(node_pair->value == NULL) handle_error("fail to malloc root value's");
  memcpy(node_pair->value, value, value_size);

  size_t data_size = sizeof(StdPairKV *);
  Node *new_node = node_create(&node_pair, data_size);
  for(int link = 0; link < 3; link++) node_set_link(new_node, NULL); // at the moment it hasn't a parent, left child and right child

  return new_node;
}

B_Tree *btree_create(void *key_root, size_t key_size, void *value_root, size_t value_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  B_Tree *new_tree = (B_Tree *) malloc(sizeof(B_Tree));
  if(new_tree == NULL) handle_error("fail to malloc binary tree");

  new_tree->root = btree_node_create(key_root, key_size, value_root, value_size);
  new_tree->vertices = 1;
  if(cmp == NULL) new_tree->cmp = btree_std_cmp;
  else new_tree->cmp = cmp;

  return new_tree;
}

void btree_delete_pair(Node *n) {
  if(n == NULL) handle_error("trying to delete a NULL node");
  StdPairKV *pair;
  node_get_value(n, &pair, sizeof(StdPairKV *));
  free(pair->key);
  pair->key = NULL;
  free(pair->value);
  pair->value = NULL;
  free(pair);
  pair = NULL;
}

void btree_delete(B_Tree *bt) {
  if(bt == NULL) handle_error("trying to delete a NULL address");
  if(bt->root != NULL) {
    node_delete_recursive(bt->root, btree_delete_pair);
    bt->root = NULL;
  }
  free(bt);
  bt = NULL;
}

void pair_get_key(StdPairKV *pair, void *key_to_ret, size_t key_size) {
  if(pair == NULL) handle_error("trying to get key from a NULL pair");
  memcpy(key_to_ret, pair->key, key_size);
}

void pair_get_value(StdPairKV *pair, void *value_to_ret, size_t value_size) {
  if(pair == NULL) handle_error("trying to get value from a NULL pair");
  memcpy(value_to_ret, pair->value, value_size);
}

void pair_get_key_and_value(StdPairKV *pair, void *key_to_ret, void *value_to_ret, size_t key_size, size_t value_size) {
  pair_get_key(pair, key_to_ret, key_size);
  pair_get_value(pair, value_to_ret, value_size);
}

int btree_std_cmp(StdPairKV *f, StdPairKV *s, size_t key_size) {
  u_int8_t bytes_f[key_size];
  u_int8_t bytes_s[key_size];

  memcpy(bytes_f, f->key, key_size);
  memcpy(bytes_s, s->key, key_size);

  for(int byte = key_size - 1; byte >= 0; byte--) {
    if(bytes_f[byte] == bytes_s[byte]) continue;
    if(bytes_f[byte] > bytes_s[byte]) return 1; //greater
    else return -1; //lesser
  }
  return 0; //equal
}

Node *btree_insert_aux(Node *node, Node *to_insert, size_t key_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  StdPairKV *pair_node, *pair_to_insert;
  node_get_value(node, &pair_node, sizeof(StdPairKV *));
  node_get_value(to_insert, &pair_to_insert, sizeof(StdPairKV *));

  int result = cmp(pair_to_insert, pair_node, key_size);
  if(!result) {
    btree_delete_pair(node);
    node_set_value(node, &pair_node, &pair_to_insert, sizeof(StdPairKV *));
    pair_node = NULL;
    node_delete(to_insert, NULL);
  }
  else if(result > 0) {
    Node *right = node_get_neighbour(node, RIGHT);
    if(right == NULL) 
      node_set_double_link_at(node, to_insert, RIGHT, PARENT);
    else
      btree_insert_aux(right, to_insert, key_size, cmp);
  }
  else {
    Node *left = node_get_neighbour(node, LEFT);
    if(left == NULL) 
      node_set_double_link_at(node, to_insert, LEFT, PARENT);
    else
      btree_insert_aux(left, to_insert, key_size, cmp);
  }
}

void btree_insert(B_Tree *bt, void *new_key, size_t key_size, void *new_value, size_t value_size) {
  if(bt == NULL) handle_error("trying to insert with NULL address");

  Node *to_insert = btree_node_create(new_key, key_size, new_value, value_size);
  if(bt->root == NULL)
    bt->root = to_insert;
  else
    btree_insert_aux(bt->root, to_insert, key_size, bt->cmp);
  bt->vertices++;
}

Node *search_aux(Node *node, StdPairKV *pair, size_t key_size, int (*cmp)(StdPairKV *, StdPairKV *, size_t)) {
  if(node == NULL) return NULL;
  StdPairKV *node_pair;
  node_get_value(node, &node_pair, sizeof(StdPairKV *));

  int result = cmp(pair, node_pair, key_size);
  if(!result) 
    return node;
  if(result > 0)
    return search_aux(node_get_neighbour(node, RIGHT), pair, key_size, cmp);
  else
    return search_aux(node_get_neighbour(node, LEFT), pair, key_size, cmp);
}

Node *btree_search_key(B_Tree *bt, void *key, size_t key_size) {
  StdPairKV *pair = (StdPairKV *) malloc(sizeof(StdPairKV));
  if(pair == NULL) handle_error("fail to create pair");
  pair->key = key;
  Node *found_node = search_aux(bt->root, pair, key_size, bt->cmp);
  free(pair);
  return found_node;
}

int btree_get_value(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size) {
  if(bt == NULL) handle_error("trying to get value with NULL address");

  Node *found_node = btree_search_key(bt, key, key_size);
  if(found_node == NULL) return -1;
  StdPairKV *pair;
  node_get_value(found_node, &pair, sizeof(StdPairKV *));
  pair_get_value(pair, to_ret, value_size);
  return 0;
}

Node *get_min(Node *node) {
  Node *min = node_get_neighbour(node, LEFT);
  if(min == NULL) return node;
  else return get_min(min);
}

// void test(B_Tree *bt) {
//   Node *n = get_min(node_get_neighbour(bt->root, RIGHT));
//   StdPairKV *pair;
//   node_get_value(n, &pair, 8);
//   int sla;
//   pair_get_value(pair, &sla, 4);
//   printf("%d\n", sla);
// }

/*
* return 1 if the tree is empty
* return 0 if removed succesfully
* return -1 if the key was not in the tree
*/
int btree_remove(B_Tree *bt, void *key, size_t key_size, void *to_ret, size_t value_size) {
  if(!bt->vertices) return 1;

  Node *found_node = btree_search_key(bt, key, key_size);
  if(found_node == NULL) return -1;
  StdPairKV *pair_to_get_value;
  node_get_value(found_node, &pair_to_get_value, sizeof(StdPairKV *));
  pair_get_value(pair_to_get_value, to_ret, value_size);
  
  Node *parent, *right, *left;
  parent = node_get_neighbour(found_node, PARENT);
  left = node_get_neighbour(found_node, LEFT);
  right = node_get_neighbour(found_node, RIGHT);
  
  if(parent == NULL) { //head deleted
    if(right == NULL and left == NULL) //all nodes will be removed
      bt->root = NULL;
    else if(right == NULL){
      bt->root = left;
      node_remove_link_at(left, PARENT);
    }
    else if(left == NULL) {
      bt->root = right;
      node_remove_link_at(right, PARENT);
    }
    else {
      bt->root = right;
      node_remove_link_at(right, PARENT);
      node_remove_link_at(left, PARENT);
      btree_insert_aux(right, left, key_size, bt->cmp);
    }
  }
  else {
    if(right == NULL and left == NULL)
      node_remove_link(parent, found_node);
    else if(right == NULL)
        node_swap_neighbours(left, found_node, PARENT, PARENT);
    else if(left == NULL)
      node_swap_neighbours(right, found_node, PARENT, PARENT);
    else {
      //swapping the value of the min node on the right of found_node with the value on found_node
      StdPairKV *min_pair, *returned_pair;
      Node *min_at_right = get_min(right);
      Node *min_parent = node_get_neighbour(min_at_right, PARENT);
      if(min_parent == found_node)
        node_swap_neighbours(min_at_right, found_node, RIGHT, RIGHT); 
      else
        node_swap_neighbours(min_at_right, min_parent, RIGHT, LEFT); //if found_node is not the parent min_at_right is the left child of his parent
      
      node_get_value(found_node, &returned_pair, sizeof(StdPairKV *));
      node_set_value(min_at_right, &min_pair, &returned_pair, sizeof(StdPairKV *));
      node_set_value(found_node, &returned_pair, &min_pair, sizeof(StdPairKV *));
      found_node = min_at_right; //to delete the min_at_right in place of found_node
    }
  }
  node_delete(found_node, btree_delete_pair);
  bt->vertices--;
  return 0;
}

unsigned get_num_pairs(B_Tree *bt) {
  if(bt == NULL) handle_error("trying to get the num of pairs on a NULL address");

  return bt->vertices;
}

void visit_aux(Node *node, void (*custom_func)(StdPairKV *)) {
  if(node == NULL) return;
  StdPairKV *pair;
  node_get_value(node, &pair, sizeof(StdPairKV *));
  custom_func(pair);
  visit_aux(node_get_neighbour(node, LEFT), custom_func);
  visit_aux(node_get_neighbour(node, RIGHT), custom_func);
}

/*
* this function pass through all pairs and calls the custom_func on every pair
*
*/
void btree_pairs_visit(B_Tree *bt, void (*custom_func)(StdPairKV *)) {
  if(bt == NULL) handle_error("trying to visit a NULL address");

  Node *tmp = bt->root;
  visit_aux(bt->root, custom_func);
}