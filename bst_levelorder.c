#include <stdio.h>
#include <stdlib.h>

// ==========================================
// 1. BST NODE STRUCTURE
// ==========================================
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, int data) {
    if (root == NULL) return createNode(data);
    if (data < root->data) root->left = insert(root->left, data);
    else if (data > root->data) root->right = insert(root->right, data);
    return root;
}

// ==========================================
// 2. QUEUE FOR LEVEL-ORDER TRAVERSAL
// A simple array-based queue of Node pointers
// ==========================================
struct Queue {
    struct Node** array;
    int front;
    int rear;
    int capacity;
};

struct Queue* createQueue(int capacity) {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->capacity = capacity;
    q->front = 0;
    q->rear = -1;
    q->array = (struct Node**)malloc(capacity * sizeof(struct Node*));
    return q;
}

int isEmpty(struct Queue* q) {
    return (q->rear < q->front);
}

void enqueue(struct Queue* q, struct Node* node) {
    q->rear++;
    q->array[q->rear] = node;
}

struct Node* dequeue(struct Queue* q) {
    return q->array[q->front++];
}

void freeQueue(struct Queue* q) {
    free(q->array);
    free(q);
}

// ==========================================
// 3. LEVEL-ORDER TRAVERSAL (BFS)
// Visits all nodes level by level, left to right.
// Time: O(N), Space: O(N) for the queue.
// ==========================================
void levelOrderTraversal(struct Node* root) {
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }

    struct Queue* q = createQueue(100);

    // Step 1: Enqueue the root
    enqueue(q, root);

    printf("Level-Order Traversal (BFS): ");

    while (!isEmpty(q)) {
        // Step 2: Dequeue and process
        struct Node* current = dequeue(q);
        printf("%d ", current->data);

        // Step 3: Enqueue children left-to-right
        if (current->left != NULL) enqueue(q, current->left);
        if (current->right != NULL) enqueue(q, current->right);
    }
    printf("\n");
    freeQueue(q);
}

// ==========================================
// 4. LEVEL-BY-LEVEL PRINTING
// Prints each level on a separate line using TWO queues
// (one for the current level, one for the next).
// ==========================================
void printLevelByLevel(struct Node* root) {
    if (root == NULL) return;

    struct Queue* current = createQueue(100);
    struct Queue* next = createQueue(100);

    enqueue(current, root);
    int level = 0;

    while (!isEmpty(current)) {
        printf("Level %d: ", level);

        while (!isEmpty(current)) {
            struct Node* node = dequeue(current);
            printf("%d ", node->data);

            if (node->left) enqueue(next, node->left);
            if (node->right) enqueue(next, node->right);
        }
        printf("\n");

        // Swap current and next queues for next level
        struct Queue* temp = current;
        current = next;
        next = temp;
        // Reset the now-empty 'next' queue (old current)
        next->front = 0;
        next->rear = -1;
        level++;
    }

    freeQueue(current);
    freeQueue(next);
}

// ==========================================
// 5. HEIGHT VIA BFS
// Calculates tree height using level-by-level counting.
// Each completed level increments the height.
// ==========================================
int findHeightBFS(struct Node* root) {
    if (root == NULL) return -1;

    struct Queue* q = createQueue(100);
    enqueue(q, root);
    int height = -1;

    while (!isEmpty(q)) {
        // Count nodes in current level
        int levelSize = q->rear - q->front + 1;
        height++;

        // Process all nodes in the current level
        for (int i = 0; i < levelSize; i++) {
            struct Node* node = dequeue(q);
            if (node->left) enqueue(q, node->left);
            if (node->right) enqueue(q, node->right);
        }
    }

    freeQueue(q);
    return height;
}

// ==========================================
// 6. WIDTH (MAX NODES AT ANY SINGLE LEVEL)
// Width = maximum number of nodes across all levels.
// ==========================================
int findWidth(struct Node* root) {
    if (root == NULL) return 0;

    struct Queue* q = createQueue(100);
    enqueue(q, root);
    int maxWidth = 0;

    while (!isEmpty(q)) {
        int levelSize = q->rear - q->front + 1;
        if (levelSize > maxWidth) maxWidth = levelSize;

        for (int i = 0; i < levelSize; i++) {
            struct Node* node = dequeue(q);
            if (node->left) enqueue(q, node->left);
            if (node->right) enqueue(q, node->right);
        }
    }

    freeQueue(q);
    return maxWidth;
}

// ==========================================
// 7. K-TH SMALLEST IN BST
// In-order traversal of BST gives sorted (ascending) output.
// The K-th node visited in-order is the K-th smallest.
// Time: O(H + K), Space: O(H) for recursion stack.
// ==========================================
struct Node* kthSmallestHelper(struct Node* root, int* count, int k) {
    if (root == NULL) return NULL;

    // Go left first (smallest values)
    struct Node* left = kthSmallestHelper(root->left, count, k);
    if (left != NULL) return left; // Found in left subtree

    // Visit this node
    (*count)++;
    if (*count == k) return root; // This is the K-th smallest!

    // Go right (larger values)
    return kthSmallestHelper(root->right, count, k);
}

struct Node* findKthSmallest(struct Node* root, int k) {
    int count = 0;
    return kthSmallestHelper(root, &count, k);
}

// ==========================================
// 8. K-TH LARGEST IN BST
// Reverse in-order (Right -> Root -> Left) gives descending order.
// The K-th node visited in reverse in-order is the K-th largest.
// ==========================================
struct Node* kthLargestHelper(struct Node* root, int* count, int k) {
    if (root == NULL) return NULL;

    // Go right first (largest values)
    struct Node* right = kthLargestHelper(root->right, count, k);
    if (right != NULL) return right;

    (*count)++;
    if (*count == k) return root;

    return kthLargestHelper(root->left, count, k);
}

struct Node* findKthLargest(struct Node* root, int k) {
    int count = 0;
    return kthLargestHelper(root, &count, k);
}

// ==========================================
// 9. MEMORY CLEANUP
// ==========================================
void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ==========================================
// 10. DRIVER CODE
// ==========================================
int main() {
    struct Node* root = NULL;

    /*
        Building the following BST:
                  50
                 /  \
               30    70
              /  \   /  \
            20   40 60   80
    */
    printf("=== BUILDING BST ===\n");
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);
    printf("Inserted: 50, 30, 70, 20, 40, 60, 80\n\n");

    printf("=== TRAVERSALS ===\n");
    levelOrderTraversal(root);

    printf("\nLevel-by-Level view:\n");
    printLevelByLevel(root);

    printf("\n=== TREE ANALYTICS ===\n");
    printf("Height (via BFS) : %d\n", findHeightBFS(root));
    printf("Width (max nodes in any level): %d\n", findWidth(root));

    printf("\n=== K-TH ORDER STATISTICS ===\n");
    struct Node* k3Small = findKthSmallest(root, 3);
    if (k3Small) printf("3rd Smallest : %d\n", k3Small->data); // Expected: 40

    struct Node* k2Large = findKthLargest(root, 2);
    if (k2Large) printf("2nd Largest  : %d\n", k2Large->data); // Expected: 70

    struct Node* k1Small = findKthSmallest(root, 1);
    if (k1Small) printf("1st Smallest : %d (minimum)\n", k1Small->data); // Expected: 20

    freeTree(root);
    return 0;
}
