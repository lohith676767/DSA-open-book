#define MAX_STATIONS 15

// Represents a tunnel to an adjacent station
struct Tunnel {
    int stationID;
    struct Tunnel* next;
};

// Represents the Metro Network
typedef struct {
    int numStations;
    struct Tunnel* stationList[MAX_STATIONS]; // Array of head pointers
} MetroNetwork;

void dispatchParamedics(MetroNetwork* grid, int startStation, int emergencyStation) {
    bool visited[MAX_STATIONS] = {false};
    int stops[MAX_STATIONS];
    int parent[MAX_STATIONS];
    int queue[MAX_STATIONS];
    int front = 0, rear = 0;

    for(int i = 0; i < MAX_STATIONS; i++) {
        stops[i] = -1; 
        parent[i] = -1;
    }

    // Initialize HQ
    queue[rear++] = startStation;
    visited[startStation] = true;
    stops[startStation] = 0;

    // Standard BFS Queue Loop
    while(front < rear) {
        int current = queue[front++];

        // Stop early if we reached the emergency
        if (current == emergencyStation) break;

        struct Tunnel* t = grid->stationList[current];
        while(t != NULL) {
            int neighbor = t->stationID;
            if(!visited[neighbor]) {
                visited[neighbor] = true;
                stops[neighbor] = stops[current] + 1;
                parent[neighbor] = current; // Record route
                queue[rear++] = neighbor;
            }
            t = t->next;
        }
    }

    printf("Minimum stops to Station %d: %d\n", emergencyStation, stops[emergencyStation]);
    // A separate recursive function would read parent[] to print: 0 -> 4 -> 9 -> 12
}


#define UNVISITED 0
#define VISITING  1
#define VISITED   2

bool redundantTunnelFound = false;

void diagnosticDFS(MetroNetwork* grid, int current, int parent, int state[]) {
    state[current] = VISITING; // Mark as currently in the recursion stack

    struct Tunnel* t = grid->stationList[current];
    while(t != NULL) {
        int neighbor = t->stationID;
        
        if (state[neighbor] == UNVISITED) {
            diagnosticDFS(grid, neighbor, current, state);
        } 
        // If we see a station we are currently exploring, and it's not where we just came from
        else if (state[neighbor] == VISITING && neighbor != parent) {
            redundantTunnelFound = true; 
            printf("Backup Route (Cycle) detected between Station %d and %d.\n", current, neighbor);
        }
        t = t->next;
    }
    state[current] = VISITED; // Mark as fully explored
}

void runDiagnostics(MetroNetwork* grid) {
    int state[MAX_STATIONS] = {UNVISITED};
    redundantTunnelFound = false;
    
    // Start diagnostic trace from HQ (Station 0)
    diagnosticDFS(grid, 0, -1, state);

    // Verify Network Integrity
    int isolatedCount = 0;
    for(int i = 0; i < grid->numStations; i++) {
        if (state[i] == UNVISITED) {
            printf("CRITICAL WARNING: Station %d is isolated from HQ!\n", i);
            isolatedCount++;
        }
    }
    
    if (isolatedCount == 0) {
        printf("Integrity Check Passed: All stations are accessible.\n");
    }
}
