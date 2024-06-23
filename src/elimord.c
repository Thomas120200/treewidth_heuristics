#include "elimord.h"

// wrapper for greedy elimination ordering alogrithm taking the optimality function as a parameter
Ordering* greedy_elimination_ordering(Graph* graph, int (*get_optimal_vertex)(Graph*)){
    Ordering* ordering = (Ordering*)malloc(sizeof(Ordering));
    ordering->ordering = (int*)malloc(sizeof(int) * graph->numVertices);
    ordering->size = 0;
    ordering->treewidth = 0;

    int iterations = graph->numVertices;

    // initialize the ordering
    for (int i = 0; i < iterations; i++) {
        ordering->ordering[i] = -1;
    }

    for(int i = 0; i < iterations; i++){
        int v = (*get_optimal_vertex)(graph);
        ordering->ordering[i] = v;
        ordering->size++;
        eliminateVertex(graph, v);
    }
    return ordering;
}

Ordering* maximum_cardinality_search(Graph* graph){
    Ordering* ordering = (Ordering*)malloc(sizeof(Ordering));
    ordering->ordering = (int*)malloc(sizeof(int) * graph->numVertices);
    ordering->size = 0;
    ordering->treewidth = 0;

    int iterations = graph->numVertices;

    // initialize the ordering
    for (int i = 0; i < iterations; i++) {
        ordering->ordering[i] = -1;
    }

    for (int i = 0; i < iterations; i++){
        int v = max_cardinality(graph, ordering);
        ordering->ordering[i] = v;
        ordering->size++;
    }
    return ordering;
}