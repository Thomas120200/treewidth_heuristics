//
// Created by Thomas Fromherz on 18.06.24.
//

#ifndef STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_ELIMORD_H
#define STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_ELIMORD_H

#include "graph_utils.h"

#define MIN_DEGREE 1
#define MIN_FILL_IN 2
#define MAX_CARDINALITY 3

Ordering* greedy_elimination_ordering(Graph* graph, int (*get_optimal_vertex)(Graph*));

Ordering* maximum_cardinality_search(Graph* graph);

#endif //STRUCTURAL_DECOMPOSITIONS_AND_ALGORITHMS_ELIMORD_H
