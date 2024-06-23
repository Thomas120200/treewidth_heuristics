//
// Created by Thomas Fromherz on 22.06.24.
//
#include "eototd.h"

void parse_args(int argc, char** argv, int* n, double* p, char** path) {
    if (argc != 4) {
        printf("Usage: ./main n p path\n");
        exit(EXIT_FAILURE);
    }
    *n = atoi(argv[1]);
    *p = atof(argv[2]);
    *path = argv[3];

}

// usage: ./main n p output_dir
int main(int argc, char** argv){

    int n;
    double p;
    char* path;
    parse_args(argc, argv, &n, &p, &path);

    Graph* graph = erdosRenyi(n, p);

    char* filename = (char*)malloc(sizeof(char) * 100);
    strcpy(filename, path);
    strcat(filename, "/graph.csv");
    graph_to_file(graph, filename);

    Ordering* ordering;
    Graph* T;


    printf("min degree\n");
    ordering = greedy_elimination_ordering(copyGraph(graph), min_degree);
    ordering_to_file(ordering, path, "min_degree");
    T = decompositionFromOrdering(copyGraph(graph), ordering, graph->numVertices);
    ordering->treewidth = getTreewidth(T);
    treewidth_to_file(ordering->treewidth, path, "min_degree");
    tree_decomposition_to_file(T, path, "min_degree");
    free(ordering);

    printf("min fill in\n");
    ordering = greedy_elimination_ordering(copyGraph(graph), min_fill_in);
    ordering_to_file(ordering, path, "min_fill_in");
    T = decompositionFromOrdering(copyGraph(graph), ordering, graph->numVertices);
    ordering->treewidth = getTreewidth(T);
    treewidth_to_file(ordering->treewidth, path, "min_fill_in");
    tree_decomposition_to_file(T, path, "min_fill_in");
    free(ordering);

    printf("max card\n");
    ordering = maximum_cardinality_search(copyGraph(graph));
    ordering_to_file(ordering, path,"max_cardinality");
    T = decompositionFromOrdering(copyGraph(graph), ordering, graph->numVertices);
    ordering->treewidth = getTreewidth(T);
    treewidth_to_file(ordering->treewidth, path, "max_cardinality");
    tree_decomposition_to_file(T, path, "max_cardinality");
    free(ordering);

    free(graph);

    free(T);

    return EXIT_SUCCESS;

    // min degree 4 min
    // min fill in more than 10 min
}