#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SESSIONS 13 // Prime table size
#define PRIME_STEP 7    // Prime < MAX_SESSIONS for Double Hashing

typedef enum { EMPTY, ACTIVE, LOGGED_OUT } SessionState;

typedef struct {
    int sessionID;      // Key
    int userID;         // Value
    SessionState state; // Slot Status
} SessionSlot;

typedef struct {
    SessionSlot slots[MAX_SESSIONS];
    int activeCount;
} CyberVaultEngine;

// Initialize Session Engine
void initEngine(CyberVaultEngine* engine) {
    engine->activeCount = 0;
    for (int i = 0; i < MAX_SESSIONS; i++) {
        engine->slots[i].state = EMPTY;
    }
}

// Primary Hash: SessionID % TableSize
int hashPrimary(int sessionID) {
    return sessionID % MAX_SESSIONS;
}

// Secondary Hash: Step size calculation (Must never evaluate to 0)
int hashSecondary(int sessionID) {
    return PRIME_STEP - (sessionID % PRIME_STEP);
}

// CREATE: Register new active user session using Double Hashing
bool createSession(CyberVaultEngine* engine, int sessionID, int userID) {
    if (engine->activeCount >= MAX_SESSIONS) return false; // Table Full

    int h1 = hashPrimary(sessionID);
    int h2 = hashSecondary(sessionID);
    int tombstoneIndex = -1;

    for (int i = 0; i < MAX_SESSIONS; i++) {
        int index = (h1 + (i * h2)) % MAX_SESSIONS;

        if (engine->slots[index].state == ACTIVE) {
            if (engine->slots[index].sessionID == sessionID) {
                engine->slots[index].userID = userID; // Session renewal
                return true;
            }
        } 
        else if (engine->slots[index].state == LOGGED_OUT) {
            if (tombstoneIndex == -1) tombstoneIndex = index; // Reuse first tombstone
        } 
        else { // Slot is EMPTY
            int target = (tombstoneIndex != -1) ? tombstoneIndex : index;
            engine->slots[target].sessionID = sessionID;
            engine->slots[target].userID = userID;
            engine->slots[target].state = ACTIVE;
            engine->activeCount++;
            return true;
        }
    }

    if (tombstoneIndex != -1) {
        engine->slots[tombstoneIndex].sessionID = sessionID;
        engine->slots[tombstoneIndex].userID = userID;
        engine->slots[tombstoneIndex].state = ACTIVE;
        engine->activeCount++;
        return true;
    }

    return false;
}

// READ: Verify and fetch User Profile ID from Session ID
int authenticateSession(CyberVaultEngine* engine, int sessionID, int* latencyProbes) {
    int h1 = hashPrimary(sessionID);
    int h2 = hashSecondary(sessionID);
    *latencyProbes = 0;

    for (int i = 0; i < MAX_SESSIONS; i++) {
        (*latencyProbes)++;
        int index = (h1 + (i * h2)) % MAX_SESSIONS;

        // Search halts on EMPTY, but bypasses LOGGED_OUT
        if (engine->slots[index].state == EMPTY) return -1; 
        
        if (engine->slots[index].state == ACTIVE && engine->slots[index].sessionID == sessionID) {
            return engine->slots[index].userID; // Session valid
        }
    }
    return -1; // Session expired or non-existent
}

// DELETE: Invalidate session on User Logout (Leaves Tombstone)
bool logoutSession(CyberVaultEngine* engine, int sessionID) {
    int h1 = hashPrimary(sessionID);
    int h2 = hashSecondary(sessionID);

    for (int i = 0; i < MAX_SESSIONS; i++) {
        int index = (h1 + (i * h2)) % MAX_SESSIONS;

        if (engine->slots[index].state == EMPTY) return false;

        if (engine->slots[index].state == ACTIVE && engine->slots[index].sessionID == sessionID) {
            engine->slots[index].state = LOGGED_OUT; // Drop Tombstone
            engine->activeCount--;
            return true;
        }
    }
    return false;
}
