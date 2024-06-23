//
// Created by Thomas Fromherz on 14.06.24.
//

#include "graph_utils.h"

////////////////////////// GRAPH CREATION //////////////////////////
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int array_size, int init_bags){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->arraySize = array_size;
    graph->numVertices = -1; // will be set later
    graph->adjLists = (Node**)malloc(array_size * sizeof(Node*));
    graph->validVertices = (int*)malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++){
        graph->adjLists[i] = NULL;
        graph->validVertices[i] = 0;
    }

    if (init_bags) {
        graph->bags = (Bag *) malloc(array_size * sizeof(Bag));
    }
    return graph;
}

Graph* erdosRenyi(int n, double p){
    srand(time(NULL));
    Graph* graph = createGraph(n, 0);
    for(int i = 0; i < n; i++){
        graph->validVertices[i] = 1;
    }
    refreshNumVertices(graph);
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            if ((double)rand() / RAND_MAX < p){
                addEdge(graph, i, j);
            }
        }
    }
    return graph;
}

Node* copyAdjList(Node* head) {
    if (head == NULL) {
        return NULL;
    }
    Node* newHead = (Node*)malloc(sizeof(Node));
    newHead->vertex = head->vertex;
    newHead->next = copyAdjList(head->next);
    return newHead;
}

Graph* copyGraph(Graph* graph) {
    if (graph == NULL) {
        return NULL;
    }

    // Create a new graph with the same size and bag initialization status
    Graph* newGraph = createGraph(graph->arraySize, graph->bags != NULL);
    newGraph->numVertices = graph->numVertices;

    // Copy adjacency lists
    for (int i = 0; i < graph->arraySize; i++) {
        newGraph->adjLists[i] = copyAdjList(graph->adjLists[i]);
        newGraph->validVertices[i] = graph->validVertices[i];
    }

    // Copy bags if they are initialized
    if (graph->bags != NULL) {
        for (int i = 0; i < graph->arraySize; i++) {
            newGraph->bags[i].size = graph->bags[i].size;
            if (graph->bags[i].size > 0) {
                newGraph->bags[i].vertices = (int*)malloc(graph->bags[i].size * sizeof(int));
                memcpy(newGraph->bags[i].vertices, graph->bags[i].vertices, graph->bags[i].size * sizeof(int));
            } else {
                newGraph->bags[i].vertices = NULL;
            }
        }
    }

    return newGraph;
}

////////////////////////// GRAPH OPERATIONS //////////////////////////

void refreshNumVertices(Graph* graph){
    int valid = 0;
    for (int i = 0; i < graph->arraySize; i++){
        if (graph->validVertices[i]){
            valid++;
        }
    }
    graph->numVertices = valid;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->validVertices[src] = 1;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
    graph->validVertices[dest] = 1;

    //update num of valid vertices
    refreshNumVertices(graph);
}


int vertexExists(Graph* graph, int vertex) {
    return graph->validVertices[vertex];
}

int edgeExists(Graph* graph, int src, int dest) {
    if(!vertexExists(graph, src) || !vertexExists(graph, dest)){
        return 0;
    }
    Node* temp = graph->adjLists[src];
    while (temp != NULL && temp->vertex != dest) {
        temp = temp->next;
    }
    return temp != NULL;
}

// removes an edge from the adjacency list of src (interprets the edge as directed!!)
void removeEdge(Graph* graph, int src, int dest) {
    Node* temp = graph->adjLists[src];
    Node* prev = NULL;
    while (temp != NULL && temp->vertex != dest) {
        prev = temp;
        temp = temp->next;
    }
    if (temp != NULL) {
        if (prev == NULL) {
            graph->adjLists[src] = temp->next;
        } else {
            prev->next = temp->next;
        }
        free(temp);
    } else {
        printf("Edge from %d to %d not found in the graph\n", src, dest);
        exit(EXIT_FAILURE);
    }
}

// removes a vertex from the graph (removes all edges from and to the vertex)
void eliminateVertex(Graph* graph, int vertex) {
    if(!vertexExists(graph, vertex)){
        printf("eliminateVertex: Vertex %d does not exist in the graph\n", vertex);
        exit(EXIT_FAILURE);
    }

    // Remove vertex from adjacency lists of its neighbors
    Node* temp = graph->adjLists[vertex];
    while (temp != NULL) {
        removeEdge(graph, temp->vertex, vertex);
        temp = temp->next;
    }

    // Add all edges between neighbors of the vertex (fill-in edges)
    Node* first_neighbor = graph->adjLists[vertex];
    while (first_neighbor != NULL) {
        Node* second_neighbor = first_neighbor->next;
        while (second_neighbor != NULL) {
            if (!edgeExists(graph, first_neighbor->vertex, second_neighbor->vertex)) {
                addEdge(graph, first_neighbor->vertex, second_neighbor->vertex);
            }
            second_neighbor = second_neighbor->next;
        }
        first_neighbor = first_neighbor->next;
    }

    // Free the vertex's adjacency list
    temp = graph->adjLists[vertex];
    while (temp != NULL) {
        Node* next = temp->next;
        free(temp);
        temp = next;
    }
    graph->adjLists[vertex] = NULL;
    graph->validVertices[vertex] = 0;
    refreshNumVertices(graph);
}

// get degree of a vertex
int degree(Graph* graph, int vertex) {
    if(!vertexExists(graph, vertex)){
        printf("degree: Vertex %d does not exist in the graph\n", vertex);
        exit(EXIT_FAILURE);
    }
    int degree = 0;
    Node* temp = graph->adjLists[vertex];
    while (temp != NULL) {
        degree++;
        temp = temp->next;
    }
    return degree;
}

// get the vertex with the minimum degree
int min_degree(Graph* graph){
    int min_degree = graph->numVertices;
    int min_vertex = -1;
    for(int i = 0; i < graph->arraySize; i++){
        if(vertexExists(graph, i) && (degree(graph, i) < min_degree)){
            min_degree = degree(graph, i);
            min_vertex = i;
        }
    }
    if (min_vertex == -1) {
        printf("unable to get vertex with min degree\n");
        exit(EXIT_FAILURE);
    }
    return min_vertex;
}

// get the vertex with minimum fill in (number of not yet existing edges between neighbors)
int min_fill_in(Graph* graph){
    int n = graph->numVertices;
    int min_fill_in = (n * (n - 1)) / 2 + 1;
    int min_vertex = -1;
    for(int i = 0; i < graph->arraySize; i++){
        if(vertexExists(graph, i)){
            int fill_in = 0;
            Node* first_neighbor = graph->adjLists[i];
            while (first_neighbor != NULL) {
                // iterate over all neighbors of i
                Node* second_neighbor = first_neighbor->next;
                while (second_neighbor != NULL) {
                    // iterate over all neighbors of the neighbor of i
                    if (!edgeExists(graph, first_neighbor->vertex, second_neighbor->vertex)) {
                        fill_in++;
                    }
                    second_neighbor = second_neighbor->next;
                }
                first_neighbor = first_neighbor->next;
            }
            if (fill_in < min_fill_in) {
                min_fill_in = fill_in;
                min_vertex = i;
            }
        }
    }
    if (min_vertex == -1) {
        printGraph(graph);
        printf("unable to get vertex with min fill in\n");
        exit(EXIT_FAILURE);
    }
    return min_vertex;
}

int contains(int* array, int size, int element){
    for (int i = 0; i < size; i++){
        if (array[i] == element){
            return 1;
        }
    }
    return 0;
}

// get node of a tree decomposition T whose bag contains a given set N_v1 of length vertices
int getContainingNode(int* set, int length, Graph* T){
    for (int i = 0; i < T->arraySize; i++){
        if (vertexExists(T, i)){
            Bag bag = T->bags[i];
            int all_found = 1;
            for (int j = 0; j < length; j++){
                if (!contains(bag.vertices, bag.size, set[j])){
                    all_found = 0;
                    break;
                }
            }
            if (all_found){
                return i;
            }

        }
    }
    printf("No node in T contains the set\n");
    exit(EXIT_FAILURE);
}

int cardinality(Graph* graph, int vertex, Ordering* ordering){
    int cardinality = 0;
    Node* neighbor = graph->adjLists[vertex];
    while (neighbor != NULL) {
        for (int i = 0; i < ordering->size; i++){
            if ((ordering->ordering[i] == neighbor->vertex) && (ordering->ordering[i] != -1)) {
                cardinality++;
            }
        }
        neighbor = neighbor->next;
    }
    return cardinality;
}

int max_cardinality(Graph* graph, Ordering* ordering){
    int max_card = -1;
    int max_vertex = -1;
    for(int i = 0; i < graph->arraySize; i++){
        if(vertexExists(graph, i) && !(contains(ordering->ordering, ordering->size, i))){
            int card = cardinality(graph, i, ordering);
            if (card > max_card) {
                max_card = card;
                max_vertex = i;
            }
        }
    }
    if (max_vertex == -1 || max_card == -1) {
        printf("unable to get vertex with max cardinality\n");
        exit(EXIT_FAILURE);
    }
    return max_vertex;
}

Bag* bagFromArray(int* array, int size){
    Bag* bag = (Bag*)malloc(sizeof(Bag));
    bag->vertices = (int*)malloc(sizeof(int) * size);
    bag->size = size;
    for (int i = 0; i < size; i++){
        bag->vertices[i] = array[i];
    }
    return bag;
}

int getTreewidth(Graph* T){
    int treewidth = 0;
    for (int i = 0; i < T->arraySize; i++){
        if (vertexExists(T, i)){
            Bag bag = T->bags[i];
            if (bag.size > treewidth){
                treewidth = bag.size;
            }
        }
    }
    return treewidth - 1;
}

// get the only valid vertex of a graph, if there is more than one valid vertex or none, the program exits
int getOnlyValidVertex(Graph* graph){
    int result = -1;
    for (int i = 0; i < graph->arraySize; i++){
        if (graph->validVertices[i]){
            if (result != -1){
                printf("More than one valid vertex found\n");
                exit(EXIT_FAILURE);
            }
            result = i;
        }
    }
    if (result == -1){
        printf("No valid vertex found\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

////////////////////////// ORDERING ///////////////////////////
Ordering* removeFirstElement(Ordering* ordering){
    Ordering* new_ordering = (Ordering*)malloc(sizeof(Ordering));
    new_ordering->ordering = (int*)malloc(sizeof(int) * (ordering->size - 1));
    new_ordering->size = ordering->size - 1;
    for (int i = 0; i < new_ordering->size; i++){
        new_ordering->ordering[i] = ordering->ordering[i + 1];
    }
    return new_ordering;
}

////////////////////////// Printing ///////////////////////////
// function for printing the graph
void printGraph(Graph* graph) {
    printf("########## Graph with %d vertices ###############\n", graph->numVertices);
    printf("Array size: %d\n", graph->arraySize);
    printf("Valid vertices: %d\n", graph->numVertices);
    int v;
    for (v = 0; v < graph->arraySize; v++) {
        if (!graph->validVertices[v]) {
            printf("vertex %d: invisible", v);
        }else{
            Node* temp = graph->adjLists[v];
            printf("vertex %d: ", v);
            while (temp) {
                printf("%d -> ", temp->vertex);
                temp = temp->next;
                if (!temp) {
                    printf("NULL");
                }
            }
            if(graph->bags){
                printf("\tbag: ");
                Bag bag = graph->bags[v];
                for (int i = 0; i < bag.size; i++) {
                    printf("%d ", bag.vertices[i]);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printOrdering(Ordering* ordering){
    printf("########## Ordering of %d vertices ###############\n", ordering->size);
    for (int i = 0; i < ordering->size; i++){
        printf("%d ", ordering->ordering[i]);
    }
    printf("\n");
}


////////////////////////// I/O ///////////////////////////
// Function to parse a single line of CSV, returns a 2 field array with src, dest
int* parse_csv_line(char *line) {
    char *token;
    char field[MAX_FIELD_LENGTH];
    int field_count = 0;
    int *ret = (int*)malloc(sizeof(int) * 2);

    // Get the first token
    token = strtok(line, ",");

    // Walk through other tokens
    while (token != NULL) {
        strncpy(field, token, MAX_FIELD_LENGTH - 1);
        field[MAX_FIELD_LENGTH - 1] = '\0';
        token = strtok(NULL, ",");
        ret[field_count++] = strtol(field, NULL, 10);
    }
    return ret;
}

int file_get_max_vertex(char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    int max_vertex = 0;
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character at the end if present
        size_t length = strlen(line);
        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        int *fields = parse_csv_line(line);
        if (fields[0] > max_vertex) {
            max_vertex = fields[0];
        }
        if (fields[1] > max_vertex) {
            max_vertex = fields[1];
        }
        free(fields);
    }
    return max_vertex;
}

Graph* file_to_graph(char *filename){
    int num_vertices = file_get_max_vertex(filename) + 1;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    Graph* graph = createGraph(num_vertices, 0);

    char line[MAX_LINE_LENGTH];

    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character at the end if present
        size_t length = strlen(line);
        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        int* fields = parse_csv_line(line);
        addEdge(graph, fields[0], fields[1]);
        free(fields);

    }
    fclose(file);
    return graph;
}

void graph_to_file(Graph* graph, char* filename){
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < graph->arraySize; i++){
        if (graph->validVertices[i]){
            Node* temp = graph->adjLists[i];
            if (temp == NULL){
                fprintf(file, "%d,,\n", i);
            }
            while (temp != NULL){
                if(temp->vertex >= i)
                    fprintf(file, "%d,%d,\n", i, temp->vertex);
                temp = temp->next;
            }
        }
    }
    fclose(file);

}

void ordering_to_file(Ordering* ordering, char* path, char* heuristic){
    char* filename = (char*)malloc(sizeof(char) * 100);
    strcpy(filename, path);
    strcat(filename, "/");
    strcat(filename, heuristic);
    strcat(filename, "_ordering.csv");
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ordering->size; i++){
        fprintf(file, "%d", ordering->ordering[i]);
        if (i < ordering->size - 1){
            fprintf(file, " ");
        }
    }
    fclose(file);

}

void treewidth_to_file(int treewidth, char* path, char* heuristic){
    char* filename = (char*)malloc(sizeof(char) * 100);
    strcpy(filename, path);
    strcat(filename, "/");
    strcat(filename, heuristic);
    strcat(filename, "_treewidth.csv");

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", treewidth);
    fclose(file);
}

void tree_decomposition_to_file(Graph* T, char* path, char* heuristic){
    char* filename = (char*)malloc(sizeof(char) * 100);
    strcpy(filename, path);
    strcat(filename, "/");
    strcat(filename, heuristic);
    strcat(filename, "_tree_decomposition.csv");
    graph_to_file(T, filename);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < T->arraySize; i++){
        if (vertexExists(T, i)){
            fprintf(file, "%d,,", i);
            Bag bag = T->bags[i];
            for (int j = 0; j < bag.size; j++){
                fprintf(file, "%d", bag.vertices[j]);
                if (j < bag.size - 1){
                    fprintf(file, ";");
                }
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}