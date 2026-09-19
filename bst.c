#include <stdio.h>
#include <stdlib.h>

// --- 1. Node Definition ---
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// --- 2. Node Creation ---
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// --- 3. Insertion ---
struct Node* insert(struct Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    
    return root; 
}

// --- 4. Search ---
struct Node* search(struct Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return search(root->left, data);
    }
    return search(root->right, data);
}

// Helper: Find minimum value (used for deletion)
struct Node* findMin(struct Node* node) {
    struct Node* current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// --- 5. Deletion ---
struct Node* deleteNode(struct Node* root, int data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } 
    else {
        // Case 1 & 2: Zero or One Child
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // Case 3: Two Children (Find In-Order Successor)
        struct Node* temp = findMin(root->right);
        root->data = temp->data; // Swap the value
        root->right = deleteNode(root->right, temp->data); // Delete the duplicate
    }
    return root;
}

// --- 6. Height Calculation ---
int findHeight(struct Node* root) {
    if (root == NULL) {
        return -1; // Return 0 if you prefer measuring by nodes instead of edges
    }

    int leftHeight = findHeight(root->left);
    int rightHeight = findHeight(root->right);

    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

// --- 7. Traversals ---
void inorderTraversal(struct Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

void preorderTraversal(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(struct Node* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->data);
    }
}

// --- 8. Memory Management ---
void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// --- 9. Driver Code ---
int main() {
    struct Node* root = NULL;

    // Building the tree
    printf("Inserting values: 50, 30, 20, 40, 70, 60, 80\n");
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    // Testing Traversals
    printf("\nIn-order Traversal (Sorted): ");
    inorderTraversal(root); 
    
    printf("\nPre-order Traversal (Copying Order): ");
    preorderTraversal(root);
    
    printf("\nPost-order Traversal (Deletion Order): ");
    postorderTraversal(root);
    printf("\n");

    // Testing Height
    printf("\nCurrent Tree Height: %d\n", findHeight(root));

    // Testing Search
    int key = 60;
    printf("\nSearching for %d... ", key);
    if (search(root, key) != NULL) {
        printf("Found!\n");
    } else {
        printf("Not found.\n");
    }

    // Testing Deletion Cases
    printf("\nDeleting 20 (Leaf node)...\n");
    root = deleteNode(root, 20);
    
    printf("Deleting 30 (Node with one child)...\n");
    // Manually making 30 have only one child for demonstration
    root = deleteNode(root, 40); 
    root = deleteNode(root, 30);

    printf("Deleting 50 (Node with two children)...\n");
    root = deleteNode(root, 50);

    printf("In-order Traversal after deletions: ");
    inorderTraversal(root);
    printf("\n");
    
    printf("New Tree Height: %d\n", findHeight(root));

    // Clean up memory
    freeTree(root);
    return 0;
}
