#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ==========================================
// 1. DATA STRUCTURE DEFINITION
// ==========================================
struct MaxHeap {
    int* array;
    int capacity;
    int size; 
};

// ==========================================
// 2. INITIALIZATION & MEMORY
// ==========================================
struct MaxHeap* createHeap(int capacity) {
    struct MaxHeap* heap = (struct MaxHeap*)malloc(sizeof(struct MaxHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int*)malloc(capacity * sizeof(int));
    return heap;
}

void freeHeap(struct MaxHeap* heap) {
    if (heap) {
        free(heap->array);
        free(heap);
    }
}

// ==========================================
// 3. INDEX & MATH HELPERS
// ==========================================
int parent(int i) { return (i - 1) / 2; }
int leftChild(int i) { return (2 * i) + 1; }
int rightChild(int i) { return (2 * i) + 2; }

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ==========================================
// 4. CORE HEAPIFY MECHANICS
// ==========================================

// Bubble UP (Used when a value becomes larger than its parent)
void heapifyUp(struct MaxHeap* heap, int index) {
    while (index != 0 && heap->array[index] > heap->array[parent(index)]) {
        swap(&heap->array[index], &heap->array[parent(index)]);
        index = parent(index);
    }
}

// Trickle DOWN (Used when a value becomes smaller than its children)
void heapifyDown(struct MaxHeap* heap, int index) {
    int maxIndex = index; 
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < heap->size && heap->array[left] > heap->array[maxIndex]) {
        maxIndex = left;
    }
    if (right < heap->size && heap->array[right] > heap->array[maxIndex]) {
        maxIndex = right;
    }
    if (maxIndex != index) {
        swap(&heap->array[index], &heap->array[maxIndex]);
        heapifyDown(heap, maxIndex); 
    }
}

// Helper heapifyDown for raw arrays (Used inside HeapSort)
void heapifyDownRaw(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapifyDownRaw(arr, n, largest);
    }
}

// ==========================================
// 5. STANDARD PRIORITY QUEUE OPERATIONS
// ==========================================

void insert(struct MaxHeap* heap, int data) {
    if (heap->size == heap->capacity) {
        printf("Heap Overflow! Cannot insert %d\n", data);
        return;
    }
    int newIndex = heap->size;
    heap->array[newIndex] = data;
    heap->size++;
    heapifyUp(heap, newIndex);
}

int extractMax(struct MaxHeap* heap) {
    if (heap->size <= 0) return -1; 
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }
    int root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1]; 
    heap->size--;
    heapifyDown(heap, 0); 
    return root;
}

int getMax(struct MaxHeap* heap) {
    if (heap->size <= 0) return -1;
    return heap->array[0];
}

// ==========================================
// 6. ADVANCED KEY MODIFICATIONS & SEARCH
// ==========================================

int search(struct MaxHeap* heap, int target) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i] == target) return i;
    }
    return -1;
}

void increaseKey(struct MaxHeap* heap, int index, int newValue) {
    if (index < 0 || index >= heap->size || newValue < heap->array[index]) return;
    heap->array[index] = newValue;
    heapifyUp(heap, index); 
}

void decreaseKey(struct MaxHeap* heap, int index, int newValue) {
    if (index < 0 || index >= heap->size || newValue > heap->array[index]) return;
    heap->array[index] = newValue;
    heapifyDown(heap, index); 
}

// Smart Key Modifier: Automatically determines whether to bubble up or trickle down
void updateKey(struct MaxHeap* heap, int index, int newValue) {
    if (index < 0 || index >= heap->size) return;
    int oldValue = heap->array[index];
    heap->array[index] = newValue;

    if (newValue > oldValue) {
        heapifyUp(heap, index);
    } else if (newValue < oldValue) {
        heapifyDown(heap, index);
    }
}

void deleteElement(struct MaxHeap* heap, int index) {
    if (index < 0 || index >= heap->size) return;
    increaseKey(heap, index, INT_MAX); // Forces element to root
    extractMax(heap);                  // Removes root
}

// ==========================================
// 7. HEAP CONSTRUCTION & MERGING
// ==========================================

// Build a heap from an unsorted array in linear time O(N)
void buildHeap(struct MaxHeap* heap, int* unsortedArray, int n) {
    if (n > heap->capacity) return;
    
    for (int i = 0; i < n; i++) {
        heap->array[i] = unsortedArray[i];
    }
    heap->size = n;

    // Bottom-up heapify from last non-leaf node down to root
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapifyDown(heap, i);
    }
}

// Merge two MaxHeaps into a single new MaxHeap
struct MaxHeap* mergeHeaps(struct MaxHeap* h1, struct MaxHeap* h2) {
    struct MaxHeap* merged = createHeap(h1->size + h2->size);
    
    for (int i = 0; i < h1->size; i++) merged->array[i] = h1->array[i];
    for (int i = 0; i < h2->size; i++) merged->array[h1->size + i] = h2->array[i];
    
    merged->size = h1->size + h2->size;

    for (int i = (merged->size / 2) - 1; i >= 0; i--) {
        heapifyDown(merged, i);
    }
    return merged;
}

// ==========================================
// 8. HEAP ALGORITHMS (HEAPSORT & VALIDATION)
// ==========================================

// In-place Heapsort algorithm: Sorts an array in O(N log N) time
void heapSort(int arr[], int n) {
    // 1. Build Max-Heap from raw array
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapifyDownRaw(arr, n, i);
    }

    // 2. Extract elements one by one from heap and place at end
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]); // Move current root to end
        heapifyDownRaw(arr, i, 0); // Call max heapify on reduced heap
    }
}

// Validate if an array or tree satisfies the Max-Heap property
int isMaxHeap(struct MaxHeap* heap, int i) {
    // Leaf nodes naturally satisfy heap property
    if (i >= (heap->size - 1) / 2) return 1;

    int left = leftChild(i);
    int right = rightChild(i);

    // If left child is larger, or right child is larger, it violates Max-Heap
    if (heap->array[i] < heap->array[left]) return 0;
    if (right < heap->size && heap->array[i] < heap->array[right]) return 0;

    // Recursively check subtrees
    return isMaxHeap(heap, left) && isMaxHeap(heap, right);
}

// Find K-th largest element using heap extraction
int findKthLargest(struct MaxHeap* heap, int k) {
    if (k <= 0 || k > heap->size) return -1;

    // Create a temporary clone heap to avoid mutating the original
    struct MaxHeap* tempHeap = createHeap(heap->size);
    buildHeap(tempHeap, heap->array, heap->size);

    int result = -1;
    for (int i = 0; i < k; i++) {
        result = extractMax(tempHeap);
    }

    freeHeap(tempHeap);
    return result;
}

// ==========================================
// 9. DISPLAY & VISUALIZATION UTILITIES
// ==========================================

void printArray(struct MaxHeap* heap) {
    printf("Flat Array: [ ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("]\n");
}

// Visualizes the heap tree structure level by level
void printHeapTree(struct MaxHeap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty.\n");
        return;
    }
    printf("--- Heap Tree Structure ---\n");
    int level = 0;
    int itemsInLevel = 1;
    int count = 0;

    while (count < heap->size) {
        printf("Level %d: ", level);
        for (int i = 0; i < itemsInLevel && count < heap->size; i++) {
            printf("%d ", heap->array[count]);
            count++;
        }
        printf("\n");
        level++;
        itemsInLevel *= 2; // Each binary tree level doubles capacity
    }
    printf("---------------------------\n");
}

// ==========================================
// 10. DRIVER CODE (TESTING ALL FEATURES)
// ==========================================
int main() {
    printf("=== 1. O(N) HEAP CONSTRUCTION & TREE VISUALIZATION ===\n");
    struct MaxHeap* heap1 = createHeap(15);
    int rawData[] = {12, 7, 25, 19, 30, 4, 15, 50};
    int n = sizeof(rawData) / sizeof(rawData[0]);
    
    buildHeap(heap1, rawData, n);
    printArray(heap1);
    printHeapTree(heap1);

    printf("\n=== 2. HEAP VALIDATION & PEERS ===\n");
    if (isMaxHeap(heap1, 0)) {
        printf("Validation: Target structure is a VALID Max-Heap.\n");
    } else {
        printf("Validation: Target structure is INVALID.\n");
    }

    printf("\n=== 3. FIND K-TH LARGEST ===\n");
    printf("3rd Largest Element: %d\n", findKthLargest(heap1, 3));

    printf("\n=== 4. SMART KEY UPDATE (updateKey) ===\n");
    int idx = search(heap1, 7);
    if (idx != -1) {
        printf("Found 7 at index %d. Updating to 60 (Triggers HeapifyUp)...\n", idx);
        updateKey(heap1, idx, 60);
        printHeapTree(heap1);
    }

    printf("\n=== 5. MERGING TWO HEAPS ===\n");
    struct MaxHeap* heap2 = createHeap(5);
    insert(heap2, 100);
    insert(heap2, 80);
    insert(heap2, 90);

    printf("Heap 2 "); printArray(heap2);
    struct MaxHeap* mergedHeap = mergeHeaps(heap1, heap2);
    printf("Merged "); printArray(mergedHeap);

    printf("\n=== 6. IN-PLACE HEAPSORT DEMO ===\n");
    int numbersToSort[] = {64, 34, 25, 12, 22, 11, 90};
    int sortLen = sizeof(numbersToSort) / sizeof(numbersToSort[0]);
    
    printf("Unsorted Array : ");
    for (int i = 0; i < sortLen; i++) printf("%d ", numbersToSort[i]);
    
    heapSort(numbersToSort, sortLen);
    
    printf("\nHeapsorted Array: ");
    for (int i = 0; i < sortLen; i++) printf("%d ", numbersToSort[i]);
    printf("\n");

    // Memory Cleanup
    freeHeap(heap1);
    freeHeap(heap2);
    freeHeap(mergedHeap);

    return 0;
}
