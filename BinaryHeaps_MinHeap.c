#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * MIN-HEAP (Minimum Priority Queue)
 *
 * KEY PROPERTY: Every parent is LESS THAN OR EQUAL to both its children.
 * The ROOT always holds the SMALLEST element.
 *
 * Contrast with Max-Heap: parent is GREATER THAN OR EQUAL to children.
 *
 * Use cases:
 *   - Dijkstra's Shortest Path algorithm
 *   - Prim's Minimum Spanning Tree
 *   - Huffman Encoding
 *   - Scheduling: process with lowest priority number first
 *   - Finding K smallest elements efficiently
 */

// ==========================================
// 1. DATA STRUCTURE DEFINITION
// ==========================================
struct MinHeap {
    int* array;
    int capacity;
    int size;
};

// ==========================================
// 2. INITIALIZATION & MEMORY
// ==========================================
struct MinHeap* createHeap(int capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int*)malloc(capacity * sizeof(int));
    return heap;
}

void freeHeap(struct MinHeap* heap) {
    if (heap) {
        free(heap->array);
        free(heap);
    }
}

// ==========================================
// 3. INDEX & MATH HELPERS
// ==========================================
int parent(int i)     { return (i - 1) / 2; }
int leftChild(int i)  { return (2 * i) + 1; }
int rightChild(int i) { return (2 * i) + 2; }

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ==========================================
// 4. CORE HEAPIFY MECHANICS (MIN VERSION)
// ==========================================

// Bubble UP: Move a new node up until parent is smaller (Min property restored).
// Used after INSERTION (new element may be smaller than its parent).
void heapifyUp(struct MinHeap* heap, int index) {
    // Stop if we reached the root OR parent is already smaller (valid min-heap)
    while (index != 0 && heap->array[index] < heap->array[parent(index)]) {
        swap(&heap->array[index], &heap->array[parent(index)]);
        index = parent(index);
    }
}

// Trickle DOWN: Push an element down until children are larger (Min property restored).
// Used after DELETION (last element placed at root may be larger than children).
void heapifyDown(struct MinHeap* heap, int index) {
    int minIndex = index;  // Assume current node is the smallest
    int left  = leftChild(index);
    int right = rightChild(index);

    // If left child exists AND is smaller than current minimum
    if (left < heap->size && heap->array[left] < heap->array[minIndex]) {
        minIndex = left;
    }

    // If right child exists AND is smaller than current minimum
    if (right < heap->size && heap->array[right] < heap->array[minIndex]) {
        minIndex = right;
    }

    // If the smallest child is smaller than the parent, swap and recurse
    if (minIndex != index) {
        swap(&heap->array[index], &heap->array[minIndex]);
        heapifyDown(heap, minIndex);
    }
}

// ==========================================
// 5. STANDARD PRIORITY QUEUE OPERATIONS
// ==========================================

// Insert: Add element at end, then bubble UP.  Time: O(log N)
void insert(struct MinHeap* heap, int data) {
    if (heap->size == heap->capacity) {
        printf("Heap Overflow! Cannot insert %d\n", data);
        return;
    }
    int newIndex = heap->size;
    heap->array[newIndex] = data;
    heap->size++;
    heapifyUp(heap, newIndex);
}

// Extract Min: Remove root (minimum), replace with last, trickle DOWN.  Time: O(log N)
int extractMin(struct MinHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap Underflow! Queue is empty.\n");
        return -1;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }

    int root = heap->array[0];             // Store the minimum to return
    heap->array[0] = heap->array[heap->size - 1]; // Move last element to root
    heap->size--;
    heapifyDown(heap, 0);                  // Restore heap property

    return root;
}

// Peek (Get Min): Return minimum without removing it.  Time: O(1)
int getMin(struct MinHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap is empty.\n");
        return -1;
    }
    return heap->array[0];
}

// ==========================================
// 6. KEY MODIFICATION OPERATIONS
// ==========================================

int search(struct MinHeap* heap, int target) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i] == target) return i;
    }
    return -1;
}

// Decrease Key: Make an existing element SMALLER.  Triggers heapifyUP.
// Common in Dijkstra's algorithm when a shorter path is found.
void decreaseKey(struct MinHeap* heap, int index, int newValue) {
    if (index < 0 || index >= heap->size || newValue > heap->array[index]) {
        printf("Invalid decrease: new value must be less than current value.\n");
        return;
    }
    heap->array[index] = newValue;
    heapifyUp(heap, index); // Smaller value may need to bubble up
}

// Increase Key: Make an existing element LARGER.  Triggers heapifyDOWN.
void increaseKey(struct MinHeap* heap, int index, int newValue) {
    if (index < 0 || index >= heap->size || newValue < heap->array[index]) {
        printf("Invalid increase: new value must be greater than current value.\n");
        return;
    }
    heap->array[index] = newValue;
    heapifyDown(heap, index); // Larger value may need to trickle down
}

// Delete Arbitrary Element: Decrease its key to -INF (forces it to root), then extract.
void deleteElement(struct MinHeap* heap, int index) {
    if (index < 0 || index >= heap->size) return;
    decreaseKey(heap, index, INT_MIN); // Bring it to the root
    extractMin(heap);                  // Remove the root
}

// ==========================================
// 7. HEAP CONSTRUCTION (LINEAR TIME)
// ==========================================

// Build Min-Heap from an unsorted array in O(N) time.
// Starts from the last non-leaf node and applies heapifyDown bottom-up.
void buildMinHeap(struct MinHeap* heap, int* unsortedArray, int n) {
    if (n > heap->capacity) return;

    for (int i = 0; i < n; i++) heap->array[i] = unsortedArray[i];
    heap->size = n;

    // Last non-leaf index = (n/2) - 1
    // Leaf nodes already satisfy heap property trivially.
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapifyDown(heap, i);
    }
}

// ==========================================
// 8. HEAP SORT (ASCENDING ORDER)
//
// A Min-Heap naturally extracts the smallest element first.
// Extracting all elements one by one gives ascending sorted order.
//
// For in-place sort with a Min-Heap approach on a raw array:
// We first build a Min-Heap, then use a different strategy.
// (Note: in-place HeapSort is typically done with Max-Heap for ascending order.
//  Here we show the educational version using extractMin to produce sorted output.)
// ==========================================

void heapSortAscending(int arr[], int n) {
    struct MinHeap* heap = createHeap(n);
    buildMinHeap(heap, arr, n);

    printf("Sorted (Ascending via Min-Heap Extract): ");
    while (heap->size > 0) {
        printf("%d ", extractMin(heap));
    }
    printf("\n");

    freeHeap(heap);
}

// ==========================================
// 9. K SMALLEST ELEMENTS
//
// Using a Min-Heap built from all N elements, extract min K times.
// Time: O(N) to build + O(K log N) for K extractions = O(N + K log N)
// ==========================================
void findKSmallest(int arr[], int n, int k) {
    if (k <= 0 || k > n) {
        printf("Invalid K.\n");
        return;
    }

    struct MinHeap* heap = createHeap(n);
    buildMinHeap(heap, arr, n);

    printf("%d Smallest Elements: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", extractMin(heap));
    }
    printf("\n");

    freeHeap(heap);
}

// ==========================================
// 10. DISPLAY UTILITIES
// ==========================================
void printArray(struct MinHeap* heap) {
    printf("Flat Array : [ ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("]\n");
}

void printHeapTree(struct MinHeap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty.\n");
        return;
    }
    printf("--- Min-Heap Tree Structure ---\n");
    int level = 0;
    int levelSize = 1;
    int count = 0;

    while (count < heap->size) {
        printf("Level %d: ", level);
        for (int i = 0; i < levelSize && count < heap->size; i++) {
            printf("%d ", heap->array[count]);
            count++;
        }
        printf("\n");
        level++;
        levelSize *= 2;
    }
    printf("-------------------------------\n");
}

// ==========================================
// 11. DRIVER CODE
// ==========================================
int main() {
    printf("=== 1. BASIC INSERT & EXTRACT-MIN ===\n");
    struct MinHeap* heap = createHeap(10);

    printf("Inserting: 30, 10, 50, 5, 20, 40\n");
    insert(heap, 30);
    insert(heap, 10);
    insert(heap, 50);
    insert(heap, 5);   // Will bubble up to root (smallest)
    insert(heap, 20);
    insert(heap, 40);

    printArray(heap);
    printHeapTree(heap);

    printf("Minimum (Peek) : %d\n", getMin(heap));     // Expected: 5

    printf("\nExtracting in sorted order (ascending):\n");
    printf("Order: ");
    while (heap->size > 0) {
        printf("%d ", extractMin(heap));  // Expected: 5 10 20 30 40 50
    }
    printf("\n");

    freeHeap(heap);

    printf("\n=== 2. O(N) BUILD MIN-HEAP ===\n");
    struct MinHeap* heap2 = createHeap(10);
    int rawData[] = {45, 20, 14, 12, 31, 7, 11, 13, 7, 9};
    int n = sizeof(rawData) / sizeof(rawData[0]);

    printf("Unsorted Input: ");
    for (int i = 0; i < n; i++) printf("%d ", rawData[i]);
    printf("\n");

    buildMinHeap(heap2, rawData, n);
    printHeapTree(heap2);
    printf("Root (Minimum) : %d\n", getMin(heap2)); // Expected: 7

    printf("\n=== 3. DECREASE KEY ===\n");
    int idx = search(heap2, 31);
    if (idx != -1) {
        printf("Found 31 at index %d. Decreasing to 2 (will bubble up)...\n", idx);
        decreaseKey(heap2, idx, 2);
        printHeapTree(heap2);
        printf("New Root (Min) : %d\n", getMin(heap2)); // Expected: 2
    }

    printf("\n=== 4. DELETE ARBITRARY ELEMENT ===\n");
    int idx2 = search(heap2, 14);
    if (idx2 != -1) {
        printf("Deleting element 14 (at index %d)...\n", idx2);
        deleteElement(heap2, idx2);
        printArray(heap2);
    }

    freeHeap(heap2);

    printf("\n=== 5. HEAP SORT (ASCENDING) ===\n");
    int toSort[] = {64, 34, 25, 12, 22, 11, 90};
    int sortLen = sizeof(toSort) / sizeof(toSort[0]);

    printf("Unsorted : ");
    for (int i = 0; i < sortLen; i++) printf("%d ", toSort[i]);
    printf("\n");

    heapSortAscending(toSort, sortLen);

    printf("\n=== 6. K SMALLEST ELEMENTS ===\n");
    int data[] = {7, 10, 4, 3, 20, 15, 8, 2};
    int dLen = sizeof(data) / sizeof(data[0]);

    printf("Array: ");
    for (int i = 0; i < dLen; i++) printf("%d ", data[i]);
    printf("\n");

    findKSmallest(data, dLen, 4); // Expected smallest 4: 2 3 4 7

    printf("\n=== 7. MAX-HEAP vs MIN-HEAP COMPARISON ===\n");
    printf("+------------------+----------------------------+----------------------------+\n");
    printf("| Property         | Max-Heap                   | Min-Heap                   |\n");
    printf("+------------------+----------------------------+----------------------------+\n");
    printf("| Root holds       | LARGEST element            | SMALLEST element           |\n");
    printf("| Heap property    | parent >= children         | parent <= children         |\n");
    printf("| Main operation   | extractMax()               | extractMin()               |\n");
    printf("| heapifyUp when   | new val > parent           | new val < parent           |\n");
    printf("| HeapSort result  | Ascending (via extractMax) | Ascending (via extractMin) |\n");
    printf("| Common use       | Priority queue (max first) | Dijkstra, Prim, scheduling |\n");
    printf("+------------------+----------------------------+----------------------------+\n");

    return 0;
}
