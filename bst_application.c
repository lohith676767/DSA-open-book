#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data Structure Definition
struct MeetingNode {
    int start_time;
    int end_time;
    char dept[50];
    struct MeetingNode* left;
    struct MeetingNode* right;
};

// Helper to create a new meeting node
struct MeetingNode* createMeeting(int start, int end, const char* dept) {
    struct MeetingNode* newNode = (struct MeetingNode*)malloc(sizeof(struct MeetingNode));
    newNode->start_time = start;
    newNode->end_time = end;
    strcpy(newNode->dept, dept);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Operation 1: Insert with conflict detection
struct MeetingNode* Request_Booking(struct MeetingNode* root, int start, int end, const char* dept) {
    if (root == NULL) {
        printf("SUCCESS: %s booked %04d - %04d\n", dept, start, end);
        return createMeeting(start, end, dept);
    }

    // Overlap condition logic
    if (start < root->end_time && end > root->start_time) {
        printf("FAILED : %s %04d - %04d overlaps with %s %04d - %04d\n", 
               dept, start, end, root->dept, root->start_time, root->end_time);
        return root;
    }

    // Branching logic
    if (end <= root->start_time) {
        root->left = Request_Booking(root->left, start, end, dept);
    } else if (start >= root->end_time) {
        root->right = Request_Booking(root->right, start, end, dept);
    }

    return root;
}

// Operation 2: Print Schedule (In-Order Traversal)
void Print_Daily_Schedule(struct MeetingNode* root) {
    if (root != NULL) {
        Print_Daily_Schedule(root->left);
        printf(" -> [%04d - %04d] : %s\n", root->start_time, root->end_time, root->dept);
        Print_Daily_Schedule(root->right);
    }
}

// Helper for Operation 3: Find Minimum node
struct MeetingNode* findMin(struct MeetingNode* node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Operation 3: Cancel Booking (Delete Node)
struct MeetingNode* Cancel_Booking(struct MeetingNode* root, int target_start) {
    if (root == NULL) {
        return root;
    }

    // Search for the booking
    if (target_start < root->start_time) {
        root->left = Cancel_Booking(root->left, target_start);
    } else if (target_start > root->start_time) {
        root->right = Cancel_Booking(root->right, target_start);
    } 
    // Target found
    else {
        // Zero or One child
        if (root->left == NULL) {
            struct MeetingNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct MeetingNode* temp = root->left;
            free(root);
            return temp;
        }

        // Two children: Get in-order successor
        struct MeetingNode* temp = findMin(root->right);
        
        // Copy successor data to current node
        root->start_time = temp->start_time;
        root->end_time = temp->end_time;
        strcpy(root->dept, temp->dept);
        
        // Delete the duplicate successor
        root->right = Cancel_Booking(root->right, temp->start_time);
    }
    return root;
}

// Helper to free memory at the end of the program
void freeSchedule(struct MeetingNode* root) {
    if (root != NULL) {
        freeSchedule(root->left);
        freeSchedule(root->right);
        free(root);
    }
}

// Main function / Driver Code
int main() {
    struct MeetingNode* root = NULL;

    printf("--- PROCESSING BOOKING REQUESTS ---\n");
    root = Request_Booking(root, 900, 1030, "HR");
    root = Request_Booking(root, 1030, 1200, "Sales");
    root = Request_Booking(root, 1000, 1100, "IT"); // Should conflict with HR and Sales
    root = Request_Booking(root, 1300, 1400, "Marketing");
    root = Request_Booking(root, 800, 900, "Executives");
    
    printf("\n--- DAILY SCHEDULE ---\n");
    Print_Daily_Schedule(root);

    printf("\n--- PROCESSING CANCELLATION ---\n");
    printf("Canceling Sales meeting at 10:30...\n");
    root = Cancel_Booking(root, 1030);

    printf("\n--- UPDATED DAILY SCHEDULE ---\n");
    Print_Daily_Schedule(root);
    
    printf("\n--- LATE BOOKING REQUEST ---\n");
    root = Request_Booking(root, 1100, 1200, "IT"); // Now this should succeed since 10:30 slot is empty

    printf("\n--- FINAL DAILY SCHEDULE ---\n");
    Print_Daily_Schedule(root);

    freeSchedule(root);
    return 0;
}
