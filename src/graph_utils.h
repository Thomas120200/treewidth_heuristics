//
// Created by Thomas Fromherz on 14.06.24.
//

#ifndef STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_GRAPH_UTILS_H
#define STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_GRAPH_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256

////////////////////////// GRAPH CREATION //////////////////////////
// Graph structure, an adjacency list should be suited best because of the sparsity of the graph and the input format
typedef struct Node {
    int vertex;
    struct Node* next;

} Node;

typedef struct Bag {
    int* vertices;
    int size;
} Bag;

typedef struct Graph {
    int numVertices;
    int arraySize;
    Node** adjLists;
    int* validVertices;
    Bag* bags;
} Graph;

typedef struct Ordering {
    int* ordering;
    int size;  // length of the ordering
    int treewidth;
} Ordering;

////////////////////////// GRAPH CREATION //////////////////////////

Node* createNode(int vertex);

Graph* createGraph(int array_size, int init_bags);

Graph* erdosRenyi(int n, double p);

Graph* copyGraph(Graph* graph);

////////////////////////// GRAPH OPERATIONS //////////////////////////
void eliminateVertex(Graph* graph, int vertex);

void addEdge(Graph* graph, int src, int dest);

void refreshNumVertices(Graph* graph);

int vertexExists(Graph* graph, int vertex);

int edgeExists(Graph* graph, int src, int dest);

int degree(Graph* graph, int vertex);

int cardinality(Graph* graph, int vertex, Ordering* ordering);

int getContainingNode(int* set, int length, Graph* T);

int min_degree(Graph* graph);

int min_fill_in(Graph* graph);

int max_cardinality(Graph* graph, Ordering* ordering);

Bag* bagFromArray(int* array, int size);

int getTreewidth(Graph* T);

int getOnlyValidVertex(Graph* graph);

////////////////////////// ORDERING //////////////////////////
Ordering* removeFirstElement(Ordering* ordering);

////////////////////////// PRINTING ///////////////////////////
void printGraph(Graph* graph);

void printOrdering(Ordering* ordering);

////////////////////////// I/O ///////////////////////////
Graph* file_to_graph(char* filename);

void graph_to_file(Graph* graph, char* filename);

void ordering_to_file(Ordering* ordering, char* path, char* heuristic);

void tree_decomposition_to_file(Graph* T, char* path, char* heuristic);

void treewidth_to_file(int treewidth, char* path, char* heuristic);


#endif //STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_GRAPH_UTILS_H
