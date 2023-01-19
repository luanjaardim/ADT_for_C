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

#endif /* GRAPH_LIB_DEF */