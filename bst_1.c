#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Required for INT_MIN and INT_MAX in isBST

// ==========================================
// 1. DATA STRUCTURE
// ==========================================
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// ==========================================
// 2. CORE OPERATIONS
// ==========================================

// Create a new node
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

// Insert a node
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

// Search for a value
struct Node* search(struct Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    }
    return search(root->right, data);
}

// Find minimum value (used for deletion)
struct Node* findMin(struct Node* node) {
    struct Node* current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Delete a node
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
        root->data = temp->data; 
        root->right = deleteNode(root->right, temp->data); 
    }
    return root;
}

// ==========================================
// 3. TREE ANALYTICS & UTILITIES
// ==========================================

// Find maximum value
struct Node* findMax(struct Node* node) {
    struct Node* current = node;
    while (current != NULL && current->right != NULL) {
        current = current->right; 
    }
    return current;
}

// Calculate height of the tree
int findHeight(struct Node* root) {
    if (root == NULL) {
        return -1; 
    }
    int leftHeight = findHeight(root->left);
    int rightHeight = findHeight(root->right);

    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

// Count total nodes
int countNodes(struct Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Count leaf nodes
int countLeaves(struct Node* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1; 
    }
    return countLeaves(root->left) + countLeaves(root->right);
}

// Find Lowest Common Ancestor (LCA)
struct Node* findLCA(struct Node* root, int n1, int n2) {
    if (root == NULL) return NULL;

    if (root->data > n1 && root->data > n2) {
        return findLCA(root->left, n1, n2);
    }
    if (root->data < n1 && root->data < n2) {
        return findLCA(root->right, n1, n2);
    }
    return root; 
}

// Validate if the tree is a strict BST
int isBSTUtil(struct Node* root, int min, int max) {
    if (root == NULL) return 1;

    if (root->data <= min || root->data >= max) {
        return 0; 
    }
    return isBSTUtil(root->left, min, root->data) && 
           isBSTUtil(root->right, root->data, max);
}

int isBST(struct Node* root) {
    return isBSTUtil(root, INT_MIN, INT_MAX);
}

// ==========================================
// 4. TRAVERSALS & MEMORY
// ==========================================

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

void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ==========================================
// 5. DRIVER CODE
// ==========================================
int main() {
    struct Node* root = NULL;

    printf("--- BUILDING THE TREE ---\n");
    printf("Inserting values: 50, 30, 20, 40, 70, 60, 80\n");
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("\n--- TRAVERSALS ---\n");
    printf("In-order   : "); inorderTraversal(root); printf("\n");
    printf("Pre-order  : "); preorderTraversal(root); printf("\n");
    printf("Post-order : "); postorderTraversal(root); printf("\n");

    printf("\n--- ANALYTICS ---\n");
    printf("Tree Height       : %d\n", findHeight(root));
    printf("Total Nodes       : %d\n", countNodes(root));
    printf("Total Leaf Nodes  : %d\n", countLeaves(root));
    
    struct Node* minNode = findMin(root);
    if (minNode) printf("Minimum Value     : %d\n", minNode->data);
    
    struct Node* maxNode = findMax(root);
    if (maxNode) printf("Maximum Value     : %d\n", maxNode->data);

    if (isBST(root)) {
        printf("Validation        : TRUE (Valid BST)\n");
    } else {
        printf("Validation        : FALSE (Invalid BST)\n");
    }

    struct Node* lca = findLCA(root, 20, 40);
    if (lca) printf("LCA of 20 & 40    : %d\n", lca->data);

    printf("\n--- DELETIONS ---\n");
    printf("Deleting 20 (Leaf)...\n");
    root = deleteNode(root, 20);
    
    printf("Deleting 70 (Node with two children)...\n");
    root = deleteNode(root, 70);

    printf("Updated In-order  : "); 
    inorderTraversal(root); 
    printf("\n");

    freeTree(root);
    return 0;
}
