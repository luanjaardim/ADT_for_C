#include "graph.h"
#include "bt_map.h"
#include "array.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct Node_Weight {
  void *node;
  void *weight;
} Node_Weight;

Node_Weight *graph_node_create(void *node, size_t node_size, void *weight, size_t weight_size) {
  if(node == NULL or weight == NULL) handle_error("trying to create node with NULL addresses");

  Node_Weight *nw = (Node_Weight *) malloc(sizeof(Node_Weight));
  if(nw == NULL) handle_error("fail to malloc graph node");

  nw->node = malloc(node_size);
  if(nw->node == NULL) handle_error("fail to malloc graph node");
  nw->weight = malloc(weight_size);
  if(nw->weight == NULL) handle_error("fail to malloc graph node");

  memcpy(nw->node, node, node_size);
  memcpy(nw->weight, weight, weight_size);

  return nw;
}

void graph_node_delete(Node_Weight *nw) {
  if(nw == NULL) handle_error("trying to delete a NULL graph node");

  free(nw->node);
  nw->node = NULL;
  free(nw->weight);
  nw->weight = NULL;
  free(nw);
}

typedef struct Graph {
  BT_Map *graph;
} Graph;

Graph *grap_create() {
  //receive a function to compare the weight
  Graph *g = (Graph *) malloc(sizeof(Graph));
  if(g == NULL) handle_error("fail to malloc graph");

  g->graph = btmap_create();
  
  return g;
}

void delete_data(BT_Pair *pair) {
  Array *ar_ret;
  pair_get_value(pair, &ar_ret, sizeof(Array *));
  unsigned len =  array_len(ar_ret);
  Node_Weight *nw;
  for(int i = 0; i < len; i++) {
    array_get_impl(ar_ret, &nw, i);
    // int deb1;
    // float deb2;
    // memcpy(&deb1, nw->node, sizeof(int));
    // memcpy(&deb2, nw->weight, sizeof(float));
    // printf("%d %f\n", deb1, deb2);
    graph_node_delete(nw);
  }
  array_delete(ar_ret);
}

void graph_delete(Graph *g) {
  if(g == NULL) handle_error("trying to delete a NULL address");

  btmap_pairs_visit(g->graph, delete_data);
  btmap_delete(g->graph);
  free(g);
}

/*
* set a simple link from n to m with the passed weight
*
*/
void graph_set_simple_link(Graph *g, void *n, void *m, size_t node_size, void *weight, size_t weight_size) {
  if(g == NULL) return;
  BT_Pair *pair = btmap_get_pair(g->graph, n, node_size);
  Node_Weight *nw = graph_node_create(m, node_size, weight, weight_size);
  
  if(pair == NULL) { //n wasn't in graph
    Array *ar_of_links = array_create(sizeof(Node_Weight *));
    array_append_impl(ar_of_links, &nw);
    btmap_insert(g->graph, n, node_size, &ar_of_links, sizeof(Array *));
  }
  else { //n already has links
    Array *ar_ret;
    pair_get_value(pair, &ar_ret, sizeof(Array *));
    array_append_with_sort(ar_ret, &nw, array_std_cmp);
  }
}

void graph_set_double_link(Graph *g, void *n, void *m, size_t node_size, void *weight, size_t weight_size) {
  if(g == NULL) return;
  graph_set_simple_link(g, n, m, node_size, weight, weight_size);
  graph_set_simple_link(g, m, n, node_size, weight, weight_size);
}

void print_aux(BT_Pair *pair) {
  int key;
  pair_get_key(pair, &key, sizeof(int));
  Array *ar;
  pair_get_value(pair, &ar, sizeof(Array *));
  unsigned len =  array_len(ar);
  printf("%d : ", key);
  for(int i = 0; i < len; i++) {
    Node_Weight *nw;
    array_get_impl(ar, &nw, i);
    printf("{ %d, %f } ", *((int *)nw->node), *((float *)nw->weight));
  }
  printf("\n");
}

void print(Graph *g) {
  BT_Map *bt = g->graph;
  btmap_pairs_visit(bt, print_aux);
}
