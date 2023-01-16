#include "bt_map.h"
#include "node.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

/*
* ADT for a Map implemented with a Binary Tree
*
*/
typedef struct BT_Map {
  Node *root;
  unsigned vertices;
  int (*cmp)(BT_Pair *, BT_Pair *, size_t);
} BT_Map;

/*
* struct that each node has in data
*
*/
typedef struct NodeData {
  BT_Pair *pair;
  int8_t height;
} NodeData;

/*
* pair of key and value
*
*/
typedef struct BT_Pair {
  void *key, *value;
} BT_Pair;

/*
* all possible links directions
*
*/
typedef enum LinkedNodes {
  PARENT,
  LEFT,
  RIGHT,
} LinkedNodes;

/*
* generate and return a BT_Map pointer to the heap allocated Map 
* cmp can receive a custom function for comparing keys
* if NULL is passed btmap_std_cmp is used by default
*/
BT_Map *btmap_create() {
  BT_Map *new_tree = (BT_Map *) malloc(sizeof(BT_Map));
  if(new_tree == NULL) handle_error("fail to malloc the map");

  new_tree->root = NULL;
  new_tree->vertices = 0;
  new_tree->cmp = btmap_std_cmp;

  return new_tree;
}

/*
* create a node that contains a pair and height, linked initially with 3 NULL addresses
*
*/
Node *btmap_node_create(void *key, size_t key_size, void *value, size_t value_size) {
  BT_Pair *node_pair = (BT_Pair *) malloc(sizeof(BT_Pair));
  if(node_pair == NULL) handle_error("fail to malloc pair");
  node_pair->key = malloc(key_size);
  if(node_pair->key == NULL) handle_error("fail to malloc pair's key");
  memcpy(node_pair->key, key, key_size);
  node_pair->value = malloc(value_size);
  if(node_pair->value == NULL) handle_error("fail to malloc pair's value");
  memcpy(node_pair->value, value, value_size);

  NodeData *data = (NodeData *) malloc(sizeof(NodeData));
  if(data == NULL) handle_error("fail to malloc data");
  data->pair = node_pair;
  data->height = 0;

  size_t data_size = sizeof(NodeData *);
  Node *new_node = node_create(&data, data_size);
  for(int link = 0; link < 3; link++) node_set_link(new_node, NULL); // at the moment it hasn't a parent, left child and right child

  return new_node;
}

/*
* generate and return a BT_Map pointer to the heap allocated Map with a initial pair
* cmp can receive a custom function for comparing keys
* if NULL is passed btmap_std_cmp is used by default
*/
BT_Map *btmap_create_with_pair(void *key_root, size_t key_size, void *value_root, size_t value_size) {
  BT_Map *new_tree = (BT_Map *) malloc(sizeof(BT_Map));
  if(new_tree == NULL) handle_error("fail to malloc map");

  new_tree->root = btmap_node_create(key_root, key_size, value_root, value_size);
  new_tree->vertices = 1;
  new_tree->cmp = btmap_std_cmp;

  return new_tree;
}

/*
* function passed to node_delete, custom deleting the data
*
*/
void btmap_delete_node_data(void *data) {
  if(data == NULL) handle_error("trying to delete a NULL node");
  NodeData *node_data;
  memcpy(&node_data, data, sizeof(NodeData *));
  BT_Pair *pair = node_data->pair;
  free(pair->key);
  pair->key = NULL;
  free(pair->value);
  pair->value = NULL;
  free(pair);
  pair = NULL;
  free(node_data);
  node_data = NULL;
}

/*
* deleting all content of map allocated on heap
*
*/
void btmap_delete(BT_Map *map) {
  if(map == NULL) handle_error("trying to delete a NULL address");
  if(map->root != NULL) {
    node_delete_recursive(map->root, btmap_delete_node_data);
    map->root = NULL;
  }
  free(map);
  map = NULL;
}

/*
* returns the height of the node, the greater path till a leaf(node without children)
*
*/
int8_t node_get_height(Node *n) {
  if(n == NULL) return -1;
  NodeData *data;
  node_get_value(n, &data, sizeof(NodeData *));
  return data->height;
}

/*
* returns the pair of n
*
*/
BT_Pair *node_get_pair(Node *n) {
  if(n == NULL) return NULL;
  NodeData *data;
  node_get_value(n, &data, sizeof(NodeData *));
  return data->pair;
}

/*
* copy the key of n's pair to key_to_ret
*
*/
void pair_get_key(BT_Pair *pair, void *key_to_ret, size_t key_size) {
  if(pair == NULL) handle_error("trying to get key from a NULL pair");
  memcpy(key_to_ret, pair->key, key_size);
}

/*
* copy the value of n's pair to value_to_ret
*
*/
void pair_get_value(BT_Pair *pair, void *value_to_ret, size_t value_size) {
  if(pair == NULL) handle_error("trying to get value from a NULL pair");
  memcpy(value_to_ret, pair->value, value_size);
}

/*
* copy both key and value of a pair to key_to_ret and value_to_ret, respectively
*
*/
void pair_get_key_and_value(BT_Pair *pair, void *key_to_ret, void *value_to_ret, size_t key_size, size_t value_size) {
  pair_get_key(pair, key_to_ret, key_size);
  pair_get_value(pair, value_to_ret, value_size);
}

/*
* auxiliar function, to recursively search for some key
*
*/
Node *search_aux(Node *node, BT_Pair *pair, size_t key_size, int (*cmp)(BT_Pair *, BT_Pair *, size_t)) {
  if(node == NULL) return NULL;
  BT_Pair *node_pair = node_get_pair(node);

  int result = cmp(pair, node_pair, key_size);
  if(!result) 
    return node;
  if(result > 0)
    return search_aux(node_get_neighbour(node, RIGHT), pair, key_size, cmp);
  else
    return search_aux(node_get_neighbour(node, LEFT), pair, key_size, cmp);
}

/*
* returns the node with the searched key, or NULL if not found
*
*/
Node *btmap_search_key(BT_Map *map, void *key, size_t key_size) {
  BT_Pair *pair = (BT_Pair *) malloc(sizeof(BT_Pair));
  if(pair == NULL) handle_error("fail to create pair");
  pair->key = key;
  Node *found_node = search_aux(map->root, pair, key_size, map->cmp);
  free(pair);
  return found_node;
}

/*
* copy the value of the pair with the correspondent key
* returns -1 -> key not found
* returns 0 -> key found
*/
int btmap_get_value(BT_Map *map, void *key, size_t key_size, void *to_ret, size_t value_size) {
  if(map == NULL or key == NULL or to_ret == NULL) handle_error("trying to get value with NULL addresses");

  Node *found_node = btmap_search_key(map, key, key_size);
  if(found_node == NULL) return -1;
  BT_Pair *pair = node_get_pair(found_node);
  pair_get_value(pair, to_ret, value_size);
  return 0;
}

/*
* standard key comparing function, NULL is passed at btmap_create function this one is used by default
* returns 0 if equal
* returns 1 if the first is greater
* returns -1 if the second is greater
*/
int btmap_std_cmp(BT_Pair *f, BT_Pair *s, size_t key_size) {
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

Node *node_right_rotate(Node *n);
Node *node_left_rotate(Node *n);
Node *balance_it_if_need(Node *n, int res, BT_Pair *to_balance, size_t key_size, int (*cmp)(BT_Pair *, BT_Pair *, size_t));

/*
* auxiliar function, to recursively compare nodes till insert
* returns NULL if the root has changed during the insertion,
* that occurs when balancing the tree
*/
Node *btmap_insert_aux(Node *node, Node *to_insert, size_t key_size, int (*cmp)(BT_Pair *, BT_Pair *, size_t)) {
  NodeData *data_node, *data_to_insert;
  BT_Pair *pair_node, *pair_to_insert;
  node_get_value(node, &data_node, sizeof(NodeData *));
  node_get_value(to_insert, &data_to_insert, sizeof(NodeData *));
  pair_node = data_node->pair;
  pair_to_insert = data_to_insert->pair;

  int result = cmp(pair_to_insert, pair_node, key_size);
  if(!result) {
    btmap_delete_node_data(node_data_pnt(node));
    node_set_value(node, &data_node, &data_to_insert, sizeof(BT_Pair *));
    node_delete(to_insert, NULL);
  }
  else if(result > 0) {
    Node *right = node_get_neighbour(node, RIGHT);
    if(right == NULL) 
      node_set_double_link_at(node, to_insert, RIGHT, PARENT);
    else
      btmap_insert_aux(right, to_insert, key_size, cmp);
  }
  else {
    Node *left = node_get_neighbour(node, LEFT);
    if(left == NULL) 
      node_set_double_link_at(node, to_insert, LEFT, PARENT);
    else
      btmap_insert_aux(left, to_insert, key_size, cmp);
  }

  return balance_it_if_need(node, result, pair_to_insert, key_size, cmp);
}

/*
* insert a key and a value as a pair on map
*
*/
void btmap_insert(BT_Map *map, void *new_key, size_t key_size, void *new_value, size_t value_size) {
  if(map == NULL or new_key == NULL or new_value == NULL) handle_error("trying to insert with NULL addresses");

  Node *to_insert = btmap_node_create(new_key, key_size, new_value, value_size);
  if(map->root == NULL)
    map->root = to_insert;
  else {
    Node *n = btmap_insert_aux(map->root, to_insert, key_size, map->cmp);
    if(n!= NULL) map->root = n; //root changed by rotations
  }
  map->vertices++;
}

/*
* recursively search for the min key after node
*
*/
Node *get_min(Node *node) {
  Node *min = node_get_neighbour(node, LEFT);
  if(min == NULL) return node;
  else return get_min(min);
}

/*
* remove a pair from the map and copy the value of the removed key to to_ret, pass NULL if don't want the copy
* return 1 if the tree is empty
* return 0 if removed succesfully
* return -1 if the key was not in the tree
*/
int btmap_remove(BT_Map *map, void *key, size_t key_size, void *to_ret, size_t value_size) {
  if(map == NULL or key == NULL) handle_error("trying to remove with a NULL address");
  if(!map->vertices) return 1;

  Node *found_node = btmap_search_key(map, key, key_size);
  if(found_node == NULL) return -1;
  BT_Pair *pair_to_get_value = node_get_pair(found_node);
  if(to_ret != NULL) pair_get_value(pair_to_get_value, to_ret, value_size);
  
  Node *parent, *right, *left;
  parent = node_get_neighbour(found_node, PARENT);
  left = node_get_neighbour(found_node, LEFT);
  right = node_get_neighbour(found_node, RIGHT);
  
  if(parent == NULL) { //head deleted
    if(right == NULL and left == NULL) //all nodes will be removed
      map->root = NULL;
    else if(right == NULL){
      map->root = left;
      node_remove_link_at(left, PARENT);
    }
    else if(left == NULL) {
      map->root = right;
      node_remove_link_at(right, PARENT);
    }
    else {
      map->root = right;
      node_remove_link_at(right, PARENT);
      node_remove_link_at(left, PARENT);
      btmap_insert_aux(right, left, key_size, map->cmp);
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
      BT_Pair *min_pair, *returned_pair;
      Node *min_at_right = get_min(right);
      Node *min_parent = node_get_neighbour(min_at_right, PARENT);
      if(min_parent == found_node)
        node_swap_neighbours(min_at_right, found_node, RIGHT, RIGHT); 
      else
        node_swap_neighbours(min_at_right, min_parent, RIGHT, LEFT); //if found_node is not the parent min_at_right is the left child of his parent
      
      returned_pair = node_get_pair(found_node);
      node_set_value(min_at_right, &min_pair, &returned_pair, sizeof(BT_Pair *));
      node_set_value(found_node, &returned_pair, &min_pair, sizeof(BT_Pair *));
      found_node = min_at_right; //to delete the min_at_right in place of found_node
    }
  }
  node_delete(found_node, btmap_delete_node_data);
  map->vertices--;
  return 0;
}

/*
* returns the number of pairs that exist on that map
*
*/
unsigned btmap_get_num_pairs(BT_Map *map) {
  if(map == NULL) handle_error("trying to get the num of pairs on a NULL address");

  return map->vertices;
}

/*
* auxiliar function to recursively visit all pair of the map
*
*/
void visit_aux(Node *node, void (*custom_func)(BT_Pair *)) {
  if(node == NULL) return;
  BT_Pair *pair = node_get_pair(node);
  custom_func(pair);
  visit_aux(node_get_neighbour(node, LEFT), custom_func);
  visit_aux(node_get_neighbour(node, RIGHT), custom_func);
}

/*
* this function pass through all pairs and calls the custom_func on every pair
*
*/
void btmap_pairs_visit(BT_Map *map, void (*custom_func)(BT_Pair *)) {
  if(map == NULL) handle_error("trying to visit a NULL address");
  visit_aux(map->root, custom_func);
}

/*
* returns the pair with the specific key
*
*/
BT_Pair *btmap_get_pair(BT_Map *map, void *key, size_t key_size) { 
  Node *found = btmap_search_key(map, key, key_size);
  return node_get_pair(found);;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
* function used to balance the tree rotate to left over a node
*
*/
Node *node_left_rotate(Node *n) {
  Node *return_node = NULL;
  Node *rn = node_get_neighbour(n, RIGHT);
  if(rn == NULL) return NULL;

  node_swap_neighbours(n, rn, RIGHT, LEFT); //rn left is now itself
  node_swap_neighbours(n, rn, PARENT, PARENT); //n parent is now itself

  //fixing n parent to rn
  node_remove_link_at(n, PARENT);
  node_set_link_at(n, rn, PARENT);
  if(node_get_neighbour(rn, PARENT) == NULL) return_node = rn; //the new root

  //fixing rn left to n
  node_remove_link_at(rn, LEFT);
  node_set_link_at(rn, n, LEFT);

  NodeData *n_data, *rn_data;
  node_get_value(n, &n_data, sizeof(NodeData *));
  node_get_value(rn, &rn_data, sizeof(NodeData *));

  //updating heights
  n_data->height = 1 + MAX(node_get_height(node_get_neighbour(n, LEFT)),
                           node_get_height(node_get_neighbour(n, RIGHT)));

  rn_data->height = 1 + MAX(node_get_height(node_get_neighbour(rn, LEFT)),
                            node_get_height(node_get_neighbour(rn, RIGHT)));

  return return_node;
}

/*
* function used to balance the tree rotate to right over a node
*
*/
Node *node_right_rotate(Node *n) {
  Node *return_node = NULL;
  Node *ln = node_get_neighbour(n, LEFT);
  if(ln == NULL) return NULL;

  node_swap_neighbours(n, ln, LEFT, RIGHT);
  node_swap_neighbours(n, ln, PARENT, PARENT);
  
  node_remove_link_at(n, PARENT);
  node_set_link_at(n, ln, PARENT);
  if(node_get_neighbour(ln, PARENT) == NULL) return_node = ln;

  node_remove_link_at(ln, RIGHT);
  node_set_link_at(ln, n, RIGHT);

  NodeData *n_data, *ln_data;
  node_get_value(n, &n_data, sizeof(NodeData *));
  node_get_value(ln, &ln_data, sizeof(NodeData *));

  //updating heights
  n_data->height = 1 + MAX(node_get_height(node_get_neighbour(n, LEFT)),
                           node_get_height(node_get_neighbour(n, RIGHT)));

  ln_data->height = 1 + MAX(node_get_height(node_get_neighbour(ln, LEFT)),
                            node_get_height(node_get_neighbour(ln, RIGHT)));

  return return_node;
}

/*
* function to check if the node need to balance and call the
* required rotations
*/
Node *balance_it_if_need(Node *n, int res, BT_Pair *to_balance, size_t key_size, int (*cmp)(BT_Pair *, BT_Pair *, size_t)) {
  if(!res) return NULL;

  NodeData *data;
  node_get_value(n, &data, sizeof(NodeData *));
  
  Node *right = node_get_neighbour(n, RIGHT), *left = node_get_neighbour(n, LEFT), *return_node = NULL;
  int8_t left_h = node_get_height(left), right_h = node_get_height(right);

  data->height = 1 + MAX(left_h, right_h);

  int8_t balance = abs(left_h - right_h);
  if(balance < 2) return return_node; //balanced

  BT_Pair *pair;
  if(res > 0) { //inserted at right
    pair = node_get_pair(right);
    int side = cmp(to_balance, pair, key_size);
    if(side > 0) //right
      return_node = node_left_rotate(n);
    else { //left
      node_right_rotate(right);
      return_node = node_left_rotate(n);
    }
  }
  else { //inserted at left
    pair = node_get_pair(left);
    int side = cmp(to_balance, pair, key_size);
    if(side < 0) //left
      return_node = node_right_rotate(n);
    else { //right
      node_left_rotate(left);
      return_node = node_right_rotate(n);
    }
  }
  //return_node will return NULL almost everytime, but if the root of the 
  //tree change during the rotatio it will return the new one
  return return_node; 
}