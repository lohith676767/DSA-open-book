#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// --- Data Structure ---
struct FlightNode {
    int time; // Time in minutes from midnight
    char flight_no[10];
    struct FlightNode* left;
    struct FlightNode* right;
    int height;
};

// --- AVL Utilities (Standard) ---
int getHeight(struct FlightNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int max(int a, int b) { return (a > b) ? a : b; }

int getBalance(struct FlightNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

struct FlightNode* createNode(int time, const char* flight_no) {
    struct FlightNode* node = (struct FlightNode*)malloc(sizeof(struct FlightNode));
    node->time = time;
    strcpy(node->flight_no, flight_no);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// --- Rotations (Standard) ---
struct FlightNode* rightRotate(struct FlightNode* y) {
    struct FlightNode* x = y->left;
    struct FlightNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

struct FlightNode* leftRotate(struct FlightNode* x) {
    struct FlightNode* y = x->right;
    struct FlightNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

// --- Operation 1: Request Landing (MODIFIED FOR ATC) ---
struct FlightNode* Request_Landing(struct FlightNode* node, int time, const char* flight_no, int K) {
    // 1. Insertion & Buffer Check
    if (node == NULL) {
        printf("Clearance Granted: %s scheduled for %04d\n", flight_no, time);
        return createNode(time, flight_no);
    }

    // ATC RULE: Check absolute difference against safety buffer K
    if (abs(time - node->time) < K) {
        printf("Clearance Denied: %s at %04d violates %d-min buffer with %s at %04d\n", 
               flight_no, time, K, node->flight_no, node->time);
        return node; // Reject insertion
    }

    if (time < node->time) {
        node->left = Request_Landing(node->left, time, flight_no, K);
    } else if (time > node->time) {
        node->right = Request_Landing(node->right, time, flight_no, K);
    }

    // 2. Update Height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get Balance
    int balance = getBalance(node);

    // 4. Rotations (Self-Balancing)
    if (balance > 1 && time < node->left->time) return rightRotate(node);
    if (balance < -1 && time > node->right->time) return leftRotate(node);
    if (balance > 1 && time > node->left->time) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && time < node->right->time) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// --- Operation 3: Display Radar ---
void Display_Radar(struct FlightNode* root) {
    if (root != NULL) {
        Display_Radar(root->left);
        printf(" -> Time: %04d | Flight: %s\n", root->time, root->flight_no);
        Display_Radar(root->right);
    }
}

// --- Driver Code ---
int main() {
    struct FlightNode* root = NULL;
    int safety_buffer = 15; // 15 minutes required between landings

    printf("--- INCOMING FLIGHT REQUESTS ---\n");
    // Notice these arrive sequentially. A BST would crash to O(N). 
    // The AVL tree will rotate to keep them O(log N).
    root = Request_Landing(root, 600, "AA101", safety_buffer);
    root = Request_Landing(root, 620, "DL202", safety_buffer);
    root = Request_Landing(root, 640, "UA303", safety_buffer);
    root = Request_Landing(root, 650, "SW404", safety_buffer); // Fails (violates UA303)
    root = Request_Landing(root, 660, "JB505", safety_buffer);

    printf("\n--- CURRENT RADAR SCHEDULE ---\n");
    Display_Radar(root);
    
    // To prove the AVL worked, print the root node. 
    // In a BST, the root would still be 600. In this AVL, it rotated to 620!
    printf("\n[System Diag] AVL Root node is currently: %04d\n", root->time);

    return 0;
}
