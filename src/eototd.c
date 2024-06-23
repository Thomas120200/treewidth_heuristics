//
// Created by Thomas Fromherz on 18.06.24.
//

#include "eototd.h"


// recursive algorithm for computing a tree decomposition from an elimination ordering
// ordering: <v1, v2, ..., vn>
Graph* decompositionFromOrdering(Graph* graph, Ordering* ordering, int originalSize){
    if (graph->numVertices == 1){
        if (ordering->size != 1){
            printf("Error: ordering size is not 1\n");
            exit(EXIT_FAILURE);
        }

        // create a tree with one node
        Graph* T = createGraph(originalSize, 1);
        T->validVertices[0] = 1;
        refreshNumVertices(T);

        // create bag for the node that contains the only left vertex of G
        if (ordering->ordering[0] != getOnlyValidVertex(graph)){
            printf("Error: ordering does not contain the only valid vertex of the graph\n");
            exit(EXIT_FAILURE);
        }
        Bag* bag = bagFromArray(ordering->ordering, 1);
        T->bags[0] = *bag;


        free(ordering->ordering);
        free(ordering);
        return T;
    }

    // store v1
    int v1 = ordering->ordering[0]; // 1

    // store N_G(v1)
    int* N_v1 = (int*)malloc(graph->numVertices * sizeof(int));
    int N_v1_size = 0;

    Node* temp = graph->adjLists[ordering->ordering[0]];
    while (temp != NULL){
        N_v1[N_v1_size] = temp->vertex;
        N_v1_size++;
        temp = temp->next;
    }

    // eliminate v1 to obtain G'
    eliminateVertex(graph, v1);

    // new ordering <v2, ..., vn>
    Ordering* new_ordering = removeFirstElement(ordering); //

    // recursive call on G' and new ordering
    Graph* T = decompositionFromOrdering(graph, new_ordering, originalSize);

    // find node t of T such that N_G(v1) is a subset of the bag of t
    // check all bags of T and find the first one that contains N_G(v1)
    int t = getContainingNode(N_v1, N_v1_size, T);

    // create new node t' (=position in the array of the adjacency list)
    int t_prime = T->numVertices;

    // create bag for t'
    int* new_bag = (int*)malloc((N_v1_size + 1) * sizeof(int));
    for (int i = 0; i < N_v1_size; i++){
        new_bag[i] = N_v1[i];
    }
    new_bag[N_v1_size] = v1;
    Bag* bag = bagFromArray(new_bag, N_v1_size + 1);
    T->bags[t_prime] = *bag;

    addEdge(T, t, t_prime);

    free(N_v1);

    return T;
}