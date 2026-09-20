#include <stdio.h>
#include <stdlib.h>

// --- Data Structure ---
struct MaxHeap {
    int* array;
    int capacity;
    int size; // Current number of elements
};

// --- Initialization ---
struct MaxHeap* createHeap(int capacity) {
    struct MaxHeap* heap = (struct MaxHeap*)malloc(sizeof(struct MaxHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int*)malloc(capacity * sizeof(int));
    return heap;
}

// --- Math Helpers ---
int parent(int i) { return (i - 1) / 2; }
int leftChild(int i) { return (2 * i) + 1; }
int rightChild(int i) { return (2 * i) + 2; }

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// --- Heapify-Up (Used for Insertion) ---
void heapifyUp(struct MaxHeap* heap, int index) {
    // While we aren't at the root, AND the current node is larger than its parent
    while (index != 0 && heap->array[index] > heap->array[parent(index)]) {
        // Swap with parent
        swap(&heap->array[index], &heap->array[parent(index)]);
        // Move our index pointer up to the parent's location to check again
        index = parent(index);
    }
}

// --- Insert Operation ---
void insert(struct MaxHeap* heap, int data) {
    if (heap->size == heap->capacity) {
        printf("Heap Overflow! Cannot insert %d\n", data);
        return;
    }

    // 1. Insert new element at the very end of the array
    int newIndex = heap->size;
    heap->array[newIndex] = data;
    heap->size++;

    // 2. Bubble it up to its correct position
    heapifyUp(heap, newIndex);
}

// --- Heapify-Down (Used for Deletion) ---
void heapifyDown(struct MaxHeap* heap, int index) {
    int maxIndex = index; // Assume current node is the largest
    int left = leftChild(index);
    int right = rightChild(index);

    // If left child exists AND is greater than current max
    if (left < heap->size && heap->array[left] > heap->array[maxIndex]) {
        maxIndex = left;
    }

    // If right child exists AND is greater than current max
    if (right < heap->size && heap->array[right] > heap->array[maxIndex]) {
        maxIndex = right;
    }

    // If the largest child is greater than the parent, we must swap and recurse
    if (maxIndex != index) {
        swap(&heap->array[index], &heap->array[maxIndex]);
        heapifyDown(heap, maxIndex); // Keep trickling down
    }
}

// --- Extract Max Operation ---
int extractMax(struct MaxHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap Underflow! Queue is empty.\n");
        return -1; 
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }

    // 1. Store the max value to return later
    int root = heap->array[0];

    // 2. Move the very last element to the root position
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    // 3. Trickle the new root down to its correct position
    heapifyDown(heap, 0);

    return root;
}

// --- Display Utility ---
void printHeap(struct MaxHeap* heap) {
    printf("Queue Array: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

// --- Driver Code ---
int main() {
    // Create a Priority Queue with capacity of 10
    struct MaxHeap* pq = createHeap(10);

    printf("Inserting priorities: 15, 30, 20, 50, 40...\n");
    insert(pq, 15);
    insert(pq, 30);
    insert(pq, 20);
    insert(pq, 50); // Will bubble up to become the root
    insert(pq, 40); 

    printHeap(pq); // Expected layout based on bubbling: 50 40 20 15 30

    printf("\nExtracting Max (highest priority item): %d\n", extractMax(pq));
    
    printf("Queue after extraction:\n");
    printHeap(pq);

    free(pq->array);
    free(pq);
    return 0;
}
