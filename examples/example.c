#include <stdio.h>
#include <stdlib.h>

// Structure to represent a node in the adjacency list
struct Node {
    int data;
    struct Node* next;
};

// Structure to represent a graph
struct Graph {
    int V;
    struct Node** adj;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to create a new graph with V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adj = (struct Node**)malloc(V * sizeof(struct Node*));

    for (int i = 0; i < V; ++i)
        graph->adj[i] = NULL;

    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int v, int w) {
    struct Node* newNode = createNode(w);
    newNode->next = graph->adj[v];
    graph->adj[v] = newNode;
}

// A recursive function used by topologicalSort
void topologicalSortUtil(struct Graph* graph, int v, int visited[], int* stack, int* stackIndex) 
{
    visited[v] = 1;

    struct Node* temp = graph->adj[v];
    while (temp != NULL) 
    {
        int adjVertex = temp->data;
        if (!visited[adjVertex]) 
        {
            topologicalSortUtil(graph, adjVertex, visited, stack, stackIndex);
        }
        temp = temp->next;
    }

    stack[(*stackIndex)++] = v;
}

// The function to do Topological Sort
void topologicalSort(struct Graph* graph) {
    int* stack = (int*)malloc(graph->V * sizeof(int));
    int stackIndex = 0;

    int* visited = (int*)malloc(graph->V * sizeof(int));
    for (int i = 0; i < graph->V; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < graph->V; ++i) {
        if (!visited[i]) {
            topologicalSortUtil(graph, i, visited, stack, &stackIndex);
        }
    }

    // Print contents of stack
    printf("Following is a Topological Sort of the given graph:\n");
    for (int i = stackIndex - 1; i >= 0; --i) {
        printf("%d ", stack[i]);
    }

    free(stack);
    free(visited);
}

// Driver Code
int main() {
    // Create a graph given in the above diagram
    struct Graph* g = createGraph(6);
    addEdge(g, 5, 2);
    addEdge(g, 5, 0);
    addEdge(g, 4, 0);
    addEdge(g, 4, 1);
    addEdge(g, 2, 3);
    addEdge(g, 3, 5);

    printf("Following is a Topological Sort of the given graph:\n");

    // Function Call
    topologicalSort(g);

    free(g->adj);
    free(g);

    printf("\n");

    return 0;
}
