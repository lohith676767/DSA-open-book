#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TABLE_SIZE 11 // Prime number for optimal distribution
#define PRIME_R 7     // Prime number < TABLE_SIZE for Double Hashing

/* --------------------------------------------------------------------------
   1. STRUCTURE DEFINITIONS & ENUMS
   -------------------------------------------------------------------------- */

// Slot status flags for Open Addressing
typedef enum { EMPTY, OCCUPIED, DELETED } SlotStatus;

// Entry format for Open Addressing tables
typedef struct {
    int key;
    int value;
    SlotStatus status;
} OpenHashEntry;

// Node format for Separate Chaining
typedef struct ChainNode {
    int key;
    int value;
    struct ChainNode* next;
} ChainNode;

// Hash Table container holding both Open Addressing and Chaining data
typedef struct {
    OpenHashEntry linearTable[TABLE_SIZE];
    OpenHashEntry quadTable[TABLE_SIZE];
    OpenHashEntry doubleTable[TABLE_SIZE];
    ChainNode* chainTable[TABLE_SIZE];
} MasterHashTable;


/* --------------------------------------------------------------------------
   2. HASH FUNCTIONS
   -------------------------------------------------------------------------- */

// Primary Hash Function: Division Method
int primaryHash(int key) {
    int hash = key % TABLE_SIZE;
    if (hash < 0) hash += TABLE_SIZE; // Handle negative keys
    return hash;
}

// Secondary Hash Function for Double Hashing
// Must never evaluate to 0 and must be relatively prime to TABLE_SIZE
int secondaryHash(int key) {
    int hash = PRIME_R - (key % PRIME_R);
    if (hash < 0) hash += PRIME_R;
    return hash;
}


/* --------------------------------------------------------------------------
   3. INITIALIZATION & MEMORY CLEANUP
   -------------------------------------------------------------------------- */

void initMasterTable(MasterHashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Open Addressing Initialization
        ht->linearTable[i].status = EMPTY;
        ht->quadTable[i].status = EMPTY;
        ht->doubleTable[i].status = EMPTY;

        // Separate Chaining Initialization
        ht->chainTable[i] = NULL;
    }
}

void freeMasterTable(MasterHashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ChainNode* current = ht->chainTable[i];
        while (current != NULL) {
            ChainNode* temp = current;
            current = current->next;
            free(temp);
        }
        ht->chainTable[i] = NULL;
    }
}


/* --------------------------------------------------------------------------
   4. OPEN ADDRESSING: LINEAR PROBING
   Formula: h(k, i) = (h1(k) + i) % TABLE_SIZE
   -------------------------------------------------------------------------- */

bool insertLinear(MasterHashTable* ht, int key, int value) {
    int h1 = primaryHash(key);
    int firstDeletedSlot = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = (h1 + i) % TABLE_SIZE;

        if (ht->linearTable[index].status == OCCUPIED) {
            if (ht->linearTable[index].key == key) {
                ht->linearTable[index].value = value; // Update value if key exists
                return true;
            }
        } 
        else if (ht->linearTable[index].status == DELETED) {
            if (firstDeletedSlot == -1) firstDeletedSlot = index; // Cache tombstone
        } 
        else { // Slot is EMPTY
            int targetIndex = (firstDeletedSlot != -1) ? firstDeletedSlot : index;
            ht->linearTable[targetIndex].key = key;
            ht->linearTable[targetIndex].value = value;
            ht->linearTable[targetIndex].status = OCCUPIED;
            return true;
        }
    }
    
    // Reuse tombstone if table was full of OCCUPIED/DELETED entries
    if (firstDeletedSlot != -1) {
        ht->linearTable[firstDeletedSlot].key = key;
        ht->linearTable[firstDeletedSlot].value = value;
        ht->linearTable[firstDeletedSlot].status = OCCUPIED;
        return true;
    }

    return false; // Table completely full
}

int searchLinear(MasterHashTable* ht, int key, int* probeCount) {
    int h1 = primaryHash(key);
    *probeCount = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        (*probeCount)++;
        int index = (h1 + i) % TABLE_SIZE;

        if (ht->linearTable[index].status == EMPTY) {
            return -1; // Not found (Search terminates on EMPTY)
        }
        if (ht->linearTable[index].status == OCCUPIED && ht->linearTable[index].key == key) {
            return ht->linearTable[index].value;
        }
        // DELETED slots are bypassed without stopping the search
    }
    return -1;
}

bool deleteLinear(MasterHashTable* ht, int key) {
    int h1 = primaryHash(key);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = (h1 + i) % TABLE_SIZE;

        if (ht->linearTable[index].status == EMPTY) {
            return false;
        }
        if (ht->linearTable[index].status == OCCUPIED && ht->linearTable[index].key == key) {
            ht->linearTable[index].status = DELETED; // Set Tombstone
            return true;
        }
    }
    return false;
}


/* --------------------------------------------------------------------------
   5. OPEN ADDRESSING: QUADRATIC PROBING
   Formula: h(k, i) = (h1(k) + i^2) % TABLE_SIZE
   -------------------------------------------------------------------------- */

bool insertQuadratic(MasterHashTable* ht, int key, int value) {
    int h1 = primaryHash(key);
    int firstDeletedSlot = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = (h1 + (i * i)) % TABLE_SIZE;

        if (ht->quadTable[index].status == OCCUPIED) {
            if (ht->quadTable[index].key == key) {
                ht->quadTable[index].value = value;
                return true;
            }
        } 
        else if (ht->quadTable[index].status == DELETED) {
            if (firstDeletedSlot == -1) firstDeletedSlot = index;
        } 
        else {
            int targetIndex = (firstDeletedSlot != -1) ? firstDeletedSlot : index;
            ht->quadTable[targetIndex].key = key;
            ht->quadTable[targetIndex].value = value;
            ht->quadTable[targetIndex].status = OCCUPIED;
            return true;
        }
    }

    if (firstDeletedSlot != -1) {
        ht->quadTable[firstDeletedSlot].key = key;
        ht->quadTable[firstDeletedSlot].value = value;
        ht->quadTable[firstDeletedSlot].status = OCCUPIED;
        return true;
    }
    return false;
}

int searchQuadratic(MasterHashTable* ht, int key, int* probeCount) {
    int h1 = primaryHash(key);
    *probeCount = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        (*probeCount)++;
        int index = (h1 + (i * i)) % TABLE_SIZE;

        if (ht->quadTable[index].status == EMPTY) return -1;
        if (ht->quadTable[index].status == OCCUPIED && ht->quadTable[index].key == key) {
            return ht->quadTable[index].value;
        }
    }
    return -1;
}


/* --------------------------------------------------------------------------
   6. OPEN ADDRESSING: DOUBLE HASHING
   Formula: h(k, i) = (h1(k) + i * h2(k)) % TABLE_SIZE
   -------------------------------------------------------------------------- */

bool insertDoubleHash(MasterHashTable* ht, int key, int value) {
    int h1 = primaryHash(key);
    int h2 = secondaryHash(key);
    int firstDeletedSlot = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = (h1 + (i * h2)) % TABLE_SIZE;

        if (ht->doubleTable[index].status == OCCUPIED) {
            if (ht->doubleTable[index].key == key) {
                ht->doubleTable[index].value = value;
                return true;
            }
        } 
        else if (ht->doubleTable[index].status == DELETED) {
            if (firstDeletedSlot == -1) firstDeletedSlot = index;
        } 
        else {
            int targetIndex = (firstDeletedSlot != -1) ? firstDeletedSlot : index;
            ht->doubleTable[targetIndex].key = key;
            ht->doubleTable[targetIndex].value = value;
            ht->doubleTable[targetIndex].status = OCCUPIED;
            return true;
        }
    }

    if (firstDeletedSlot != -1) {
        ht->doubleTable[firstDeletedSlot].key = key;
        ht->doubleTable[firstDeletedSlot].value = value;
        ht->doubleTable[firstDeletedSlot].status = OCCUPIED;
        return true;
    }
    return false;
}

int searchDoubleHash(MasterHashTable* ht, int key, int* probeCount) {
    int h1 = primaryHash(key);
    int h2 = secondaryHash(key);
    *probeCount = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        (*probeCount)++;
        int index = (h1 + (i * h2)) % TABLE_SIZE;

        if (ht->doubleTable[index].status == EMPTY) return -1;
        if (ht->doubleTable[index].status == OCCUPIED && ht->doubleTable[index].key == key) {
            return ht->doubleTable[index].value;
        }
    }
    return -1;
}


/* --------------------------------------------------------------------------
   7. SEPARATE CHAINING (OPEN HASHING)
   -------------------------------------------------------------------------- */

bool insertChaining(MasterHashTable* ht, int key, int value) {
    int index = primaryHash(key);
    
    // Check if key already exists (Update value)
    ChainNode* current = ht->chainTable[index];
    while (current != NULL) {
        if (current->key == key) {
            current->value = value;
            return true;
        }
        current = current->next;
    }

    // Insert new node at Head -> O(1)
    ChainNode* newNode = (ChainNode*)malloc(sizeof(ChainNode));
    if (!newNode) return false;
    
    newNode->key = key;
    newNode->value = value;
    newNode->next = ht->chainTable[index];
    ht->chainTable[index] = newNode;
    return true;
}

int searchChaining(MasterHashTable* ht, int key, int* probeCount) {
    int index = primaryHash(key);
    *probeCount = 0;

    ChainNode* current = ht->chainTable[index];
    while (current != NULL) {
        (*probeCount)++;
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1;
}

bool deleteChaining(MasterHashTable* ht, int key) {
    int index = primaryHash(key);
    ChainNode* current = ht->chainTable[index];
    ChainNode* prev = NULL;

    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                ht->chainTable[index] = current->next; // Deleting head
            } else {
                prev->next = current->next; // Bypass current node
            }
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}


/* --------------------------------------------------------------------------
   8. DIAGNOSTIC PRINT FUNCTIONS
   -------------------------------------------------------------------------- */

void printOpenTable(OpenHashEntry table[], const char* name) {
    printf("\n=== %s TABLE ===\n", name);
    printf("Slot | Status   | Key  | Value\n");
    printf("-------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%4d | ", i);
        if (table[i].status == OCCUPIED) {
            printf("OCCUPIED | %4d | %5d\n", table[i].key, table[i].value);
        } else if (table[i].status == DELETED) {
            printf("DELETED  | ---- | -----\n");
        } else {
            printf("EMPTY    | ---- | -----\n");
        }
    }
}

void printChainTable(MasterHashTable* ht) {
    printf("\n=== SEPARATE CHAINING TABLE ===\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Slot %2d: ", i);
        ChainNode* current = ht->chainTable[i];
        if (current == NULL) {
            printf("NULL");
        }
        while (current != NULL) {
            printf("-> [K:%d, V:%d] ", current->key, current->value);
            current = current->next;
        }
        printf("\n");
    }
}


/* --------------------------------------------------------------------------
   9. MAIN DRIVER PROGRAM
   -------------------------------------------------------------------------- */

int main() {
    MasterHashTable ht;
    initMasterTable(&ht);

    // Test keys carefully chosen to trigger collisions at index 0 (0 % 11 = 0, 11 % 11 = 0, 22 % 11 = 0, 33 % 11 = 0)
    int testKeys[] = {0, 11, 22, 33, 5};
    int numKeys = sizeof(testKeys) / sizeof(testKeys[0]);

    printf("Inserting keys yielding identical initial hash (0 %% 11 = 0):\n");
    for (int i = 0; i < numKeys; i++) {
        int key = testKeys[i];
        int val = (i + 1) * 100;
        insertLinear(&ht, key, val);
        insertQuadratic(&ht, key, val);
        insertDoubleHash(&ht, key, val);
        insertChaining(&ht, key, val);
    }

    // Output table states across all strategies
    printOpenTable(ht.linearTable, "LINEAR PROBING");
    printOpenTable(ht.quadTable, "QUADRATIC PROBING");
    printOpenTable(ht.doubleTable, "DOUBLE HASHING");
    printChainTable(&ht);

    // Demonstrate Search and Probe Performance
    printf("\n=== SEARCH DIAGNOSTICS FOR KEY 33 ===\n");
    int probes;
    
    int valL = searchLinear(&ht, 33, &probes);
    printf("Linear Probing   : Val = %d, Probes = %d\n", valL, probes);
    
    int valQ = searchQuadratic(&ht, 33, &probes);
    printf("Quadratic Probing: Val = %d, Probes = %d\n", valQ, probes);
    
    int valD = searchDoubleHash(&ht, 33, &probes);
    printf("Double Hashing   : Val = %d, Probes = %d\n", valD, probes);
    
    int valC = searchChaining(&ht, 33, &probes);
    printf("Separate Chaining: Val = %d, Probes = %d\n", valC, probes);

    // Demonstrate Tombstone Handling
    printf("\n=== DELETION & TOMBSTONE DEMO (Linear Probing) ===\n");
    printf("Deleting key 11...\n");
    deleteLinear(&ht, 11); // Slot 1 becomes DELETED
    printOpenTable(ht.linearTable, "LINEAR PROBING (Post Delete)");

    valL = searchLinear(&ht, 33, &probes);
    printf("Searching for Key 33 past Tombstone: Val = %d, Probes = %d\n", valL, probes);

    printf("\nRe-inserting Key 99 (99 %% 11 = 0)...\n");
    insertLinear(&ht, 99, 999); // Should reuse slot 1 (previously DELETED)
    printOpenTable(ht.linearTable, "LINEAR PROBING (Post Re-insertion)");

    freeMasterTable(&ht);
    return 0;
}
