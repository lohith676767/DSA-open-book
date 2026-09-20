#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXV 20
#define INF 99999

/* --------------------------------------------------------------------------
   1. STRUCTURE DEFINITIONS (Representation & Preliminaries)
   -------------------------------------------------------------------------- */

struct AdjNode {
    int vertex;
    int weight;
    struct AdjNode* next;
};

typedef struct {
    int numVertices;
    bool isDirected;
    
    // Core Representations
    int matrix[MAXV][MAXV];
    struct AdjNode* adjList[MAXV];
    
    // Preliminaries: Vertex Properties
    int inDegree[MAXV];
    int outDegree[MAXV];
} Graph;

/* --------------------------------------------------------------------------
   2. INITIALIZATION & BUILDING
   -------------------------------------------------------------------------- */

void initGraph(Graph* g, int vertices, bool directed) {
    g->numVertices = vertices;
    g->isDirected = directed;
    
    for (int i = 0; i < vertices; i++) {
        g->adjList[i] = NULL;
        g->inDegree[i] = 0;
        g->outDegree[i] = 0;
        for (int j = 0; j < vertices; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

void addEdge(Graph* g, int u, int v, int weight) {
    // Adjacency Matrix Update
    g->matrix[u][v] = weight;
    
    // Adjacency List Update (Insert at Head)
    struct AdjNode* newNode = (struct AdjNode*)malloc(sizeof(struct AdjNode));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = g->adjList[u];
    g->adjList[u] = newNode;

    // Degree Updates
    g->outDegree[u]++;
    g->inDegree[v]++;

    // Undirected Graph Adjustments
    if (!g->isDirected) {
        g->matrix[v][u] = weight;
        
        struct AdjNode* backNode = (struct AdjNode*)malloc(sizeof(struct AdjNode));
        backNode->vertex = u;
        backNode->weight = weight;
        backNode->next = g->adjList[v];
        g->adjList[v] = backNode;
        
        g->outDegree[v]++;
        g->inDegree[u]++;
    }
}

void printGraphStats(Graph* g) {
    printf("\n--- GRAPH PRELIMINARIES & REPRESENTATION ---\n");
    for (int i = 0; i < g->numVertices; i++) {
        printf("Vertex %d [In: %d, Out: %d]: ", i, g->inDegree[i], g->outDegree[i]);
        struct AdjNode* current = g->adjList[i];
        while (current != NULL) {
            printf("-> (V:%d, W:%d) ", current->vertex, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

/* --------------------------------------------------------------------------
   3. PATH RECONSTRUCTION (Post-Traversal Helper)
   -------------------------------------------------------------------------- */

void printPath(int start, int target, int parent[]) {
    if (target == start) {
        printf("%d", start);
    } else if (parent[target] == -1) {
        printf("No path exists from %d to %d", start, target);
    } else {
        printPath(start, parent[target], parent);
        printf(" -> %d", target);
    }
}

/* --------------------------------------------------------------------------
   4. BREADTH-FIRST SEARCH (Queue + Bipartite Check + Shortest Path)
   -------------------------------------------------------------------------- */

void masterBFS(Graph* g, int startVertex) {
    bool visited[MAXV] = {false};
    int distance[MAXV];
    int parent[MAXV];
    int bipColor[MAXV]; // Array for 2-coloring (0 or 1)
    
    bool isBipartite = true;
    int queue[MAXV];
    int front = 0, rear = 0;

    for (int i = 0; i < MAXV; i++) { 
        distance[i] = INF; 
        parent[i] = -1; 
        bipColor[i] = -1; // Uncolored
    }

    printf("\n--- BFS TRAVERSAL (Shortest Path & Bipartite Check) ---\nOrder: ");
    
    // Ensure disconnected components are visited
    for (int i = 0; i < g->numVertices; i++) {
        int currentStart = (i == 0) ? startVertex : i; // Start from specified vertex first
        
        if (!visited[currentStart]) {
            queue[rear++] = currentStart;
            visited[currentStart] = true;
            distance[currentStart] = 0;
            bipColor[currentStart] = 0; // Paint first node color 0

            while (front < rear) {
                int u = queue[front++];
                printf("%d ", u);

                struct AdjNode* temp = g->adjList[u];
                while (temp != NULL) {
                    int v = temp->vertex;
                    
                    if (!visited[v]) {
                        visited[v] = true;
                        distance[v] = distance[u] + 1;
                        parent[v] = u;
                        bipColor[v] = 1 - bipColor[u]; // Alternate color
                        queue[rear++] = v;
                    } 
                    // If neighbor is visited and has the SAME color, graph is not bipartite
                    else if (bipColor[v] == bipColor[u]) {
                        isBipartite = false;
                    }
                    temp = temp->next;
                }
            }
        }
    }
    
    printf("\nBipartite Status: %s\n", isBipartite ? "YES (Valid 2-Coloring)" : "NO (Odd-length cycle detected)");
    printf("Shortest Path from %d to %d: ", startVertex, g->numVertices - 1);
    printPath(startVertex, g->numVertices - 1, parent);
    printf("\n");
}

/* --------------------------------------------------------------------------
   5. DEPTH-FIRST SEARCH (Recursion + Cycle Detection + Topological Sort)
   -------------------------------------------------------------------------- */

#define WHITE 0 // Unvisited
#define GRAY  1 // Visiting
#define BLACK 2 // Visited

int timer = 0;
int topoStack[MAXV];
int topoTop = -1;
bool hasCycle = false;

void DFS_Util(Graph* g, int u, int color[], int parent[], int entry[], int exit[]) {
    color[u] = GRAY;
    entry[u] = ++timer;
    printf("%d ", u);

    struct AdjNode* temp = g->adjList[u];
    while (temp != NULL) {
        int v = temp->vertex;
        
        if (color[v] == WHITE) {
            parent[v] = u;
            DFS_Util(g, v, color, parent, entry, exit);
        } 
        else if (color[v] == GRAY) {
            if (g->isDirected || parent[u] != v) {
                hasCycle = true; // Back-edge detected
            }
        }
        temp = temp->next;
    }

    color[u] = BLACK;
    exit[u] = ++timer;
    topoStack[++topoTop] = u; // Push to stack upon finishing
}

void masterDFS(Graph* g) {
    int color[MAXV] = {WHITE};
    int parent[MAXV];
    int entry[MAXV] = {0}, exit[MAXV] = {0};
    
    timer = 0;
    topoTop = -1;
    hasCycle = false;

    for (int i = 0; i < MAXV; i++) { parent[i] = -1; }

    printf("\n--- RECURSIVE DFS (Cycles & Topological Sort) ---\nOrder: ");
    for (int i = 0; i < g->numVertices; i++) {
        if (color[i] == WHITE) {
            DFS_Util(g, i, color, parent, entry, exit);
        }
    }
    
    printf("\nCycle Detected: %s\n", hasCycle ? "YES" : "NO");
    
    if (g->isDirected && !hasCycle) {
        printf("Topological Sort Order: ");
        while (topoTop != -1) {
            printf("%d ", topoStack[topoTop--]);
        }
        printf("\n");
    } else if (g->isDirected && hasCycle) {
        printf("Topological Sort: NOT POSSIBLE (Graph is not a DAG)\n");
    }
}

/* --------------------------------------------------------------------------
   6. DRIVER FUNCTION
   -------------------------------------------------------------------------- */

int main() {
    Graph dag;
    // Create a Directed Acyclic Graph (DAG) for perfect Topological Sorting
    initGraph(&dag, 6, true);

    addEdge(&dag, 5, 2, 1);
    addEdge(&dag, 5, 0, 1);
    addEdge(&dag, 4, 0, 1);
    addEdge(&dag, 4, 1, 1);
    addEdge(&dag, 2, 3, 1);
    addEdge(&dag, 3, 1, 1);

    printGraphStats(&dag);
    
    // Start BFS from Vertex 5
    masterBFS(&dag, 5); 
    
    // Run DFS (Will generate Topo Sort)
    masterDFS(&dag);

    // Memory cleanup omitted for brevity, but standard traversal of adjList applies.
    return 0;
}
