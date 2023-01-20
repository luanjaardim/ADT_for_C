#ifndef GRAPH_LIB_DEF
#define GRAPH_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct Graph Graph;

Graph *grap_create();
void graph_delete(Graph *g);
void graph_set_simple_link(Graph *g, void *n, void *m, size_t node_size, void *weight, size_t weight_size);
void graph_set_double_link(Graph *g, void *n, void *m, size_t node_size, void *weight, size_t weight_size);
void print(Graph *g);

#ifndef INIT_GRAPH_TYPE
#define INIT_GRAPH_TYPE(name, node_type, weight_type) \
size_t name##_node_size = sizeof(node_type), name##_weight_size = sizeof(weight_type); \
void name##_graph_set_simple_link(Graph *g, node_type n, node_type m, weight_type weight) { \
  graph_set_simple_link(g, (void *) &n, (void *) &m, name##_node_size, (void *) &weight, name##_weight_size); \
} \
void name##_graph_set_double_link(Graph *g, node_type n, node_type m, weight_type weight) { \
  graph_set_double_link(g, (void *) &n, (void *) &m, name##_node_size, (void *) &weight, name##_weight_size); \
}

#endif /* INIT_GRAPH_TYPE */

#endif /* GRAPH_LIB_DEF */