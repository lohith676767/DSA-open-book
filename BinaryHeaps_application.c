#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 1. Modified Data Structure ---
struct Patient {
    int id;
    char name[50];
    int severity; // This is our HEAP KEY
};

struct ER_Triage {
    struct Patient* array; // Array of objects, not ints
    int capacity;
    int size;
};

// --- 2. Math & Swap Helpers ---
int parent(int i) { return (i - 1) / 2; }
int leftChild(int i) { return (2 * i) + 1; }
int rightChild(int i) { return (2 * i) + 2; }

// Swap entire patient records
void swapPatients(struct Patient* a, struct Patient* b) {
    struct Patient temp = *a;
    *a = *b;
    *b = temp;
}

// --- 3. Modified Heapify Operations ---
void heapifyUp(struct ER_Triage* heap, int index) {
    // Compare based on SEVERITY score
    while (index != 0 && heap->array[index].severity > heap->array[parent(index)].severity) {
        swapPatients(&heap->array[index], &heap->array[parent(index)]);
        index = parent(index);
    }
}

void heapifyDown(struct ER_Triage* heap, int index) {
    int maxIndex = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < heap->size && heap->array[left].severity > heap->array[maxIndex].severity) {
        maxIndex = left;
    }
    if (right < heap->size && heap->array[right].severity > heap->array[maxIndex].severity) {
        maxIndex = right;
    }
    if (maxIndex != index) {
        swapPatients(&heap->array[index], &heap->array[maxIndex]);
        heapifyDown(heap, maxIndex);
    }
}

// --- 4. ER Specific Operations ---

void Admit_Patient(struct ER_Triage* heap, int id, const char* name, int severity) {
    if (heap->size == heap->capacity) {
        printf("ER is at full capacity!\n");
        return;
    }
    int newIndex = heap->size;
    heap->array[newIndex].id = id;
    strcpy(heap->array[newIndex].name, name);
    heap->array[newIndex].severity = severity;
    heap->size++;
    
    // Bubble the patient up to their correct priority slot
    heapifyUp(heap, newIndex);
    printf("Admitted: %s (Severity: %d)\n", name, severity);
}

void Treat_Next(struct ER_Triage* heap) {
    if (heap->size <= 0) {
        printf("Waiting room is empty.\n");
        return;
    }
    
    struct Patient critical = heap->array[0]; // Highest severity is at root
    
    // Standard extraction
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    
    printf("\n>>> NOW TREATING: %s (ID: %d, Severity: %d) <<<\n", 
           critical.name, critical.id, critical.severity);
}

void Code_Blue(struct ER_Triage* heap, int target_id) {
    int index = -1;
    // O(N) Search for patient
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i].id == target_id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Code Blue Failed: Patient ID %d not found.\n", target_id);
        return;
    }
    
    printf("\n!!! CODE BLUE CALLED FOR %s !!!\n", heap->array[index].name);
    // 1. Instantly increase key
    heap->array[index].severity = 999;
    // 2. Bubble up to the root
    heapifyUp(heap, index);
}

void Print_Waiting_Room(struct ER_Triage* heap) {
    printf("\nCurrent Waiting Room Queue (Heap Array Layout):\n");
    for (int i = 0; i < heap->size; i++) {
        printf("[%s: %d] ", heap->array[i].name, heap->array[i].severity);
    }
    printf("\n");
}

// --- 5. Driver Code ---
int main() {
    struct ER_Triage* er = (struct ER_Triage*)malloc(sizeof(struct ER_Triage));
    er->capacity = 10;
    er->size = 0;
    er->array = (struct Patient*)malloc(er->capacity * sizeof(struct Patient));

    printf("--- MORNING SHIFT ADMISSIONS ---\n");
    Admit_Patient(er, 101, "John (Sprained Ankle)", 20);
    Admit_Patient(er, 102, "Mary (Mild Fever)", 30);
    Admit_Patient(er, 103, "Bob (Chest Pain)", 80);
    Admit_Patient(er, 104, "Alice (Broken Arm)", 50);

    Print_Waiting_Room(er); // Bob will be at the root (index 0)

    // Mary suddenly collapses in the waiting room
    Code_Blue(er, 102); 
    
    Print_Waiting_Room(er); // Mary is now at the root with 999 severity

    // Doctor calls for the next patients
    Treat_Next(er); // Will treat Mary
    Treat_Next(er); // Will treat Bob

    free(er->array);
    free(er);
    return 0;
}
