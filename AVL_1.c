#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ==========================================
// 1. DATA STRUCTURE
// ==========================================
struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height; 
};

// ==========================================
// 2. AVL UTILITIES & ROTATIONS
// ==========================================

int getHeight(struct AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(struct AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

struct AVLNode* createNode(int data) {
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

struct AVLNode* rightRotate(struct AVLNode* y) {
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

struct AVLNode* leftRotate(struct AVLNode* x) {
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// ==========================================
// 3. CORE OPERATIONS (INSERT, DELETE, SEARCH)
// ==========================================

struct AVLNode* insertAVL(struct AVLNode* node, int data) {
    if (node == NULL) return createNode(data);

    if (data < node->data) node->left = insertAVL(node->left, data);
    else if (data > node->data) node->right = insertAVL(node->right, data);
    else return node; // No duplicates allowed

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    // 4 Rotation Cases
    if (balance > 1 && data < node->left->data) return rightRotate(node);
    if (balance < -1 && data > node->right->data) return leftRotate(node);
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct AVLNode* search(struct AVLNode* root, int data) {
    if (root == NULL || root->data == data) return root;
    if (data < root->data) return search(root->left, data);
    return search(root->right, data);
}

struct AVLNode* findMin(struct AVLNode* node) {
    struct AVLNode* current = node;
    while (current != NULL && current->left != NULL)
        current = current->left;
    return current;
}

struct AVLNode* deleteAVL(struct AVLNode* root, int data) {
    // 1. Standard BST Deletion
    if (root == NULL) return root;

    if (data < root->data) {
        root->left = deleteAVL(root->left, data);
    } else if (data > root->data) {
        root->right = deleteAVL(root->right, data);
    } else {
        if (root->left == NULL) {
            struct AVLNode* temp = root->right;
            free(root);
            return temp; 
        } else if (root->right == NULL) {
            struct AVLNode* temp = root->left;
            free(root);
            return temp;
        }
        struct AVLNode* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteAVL(root->right, temp->data);
    }

    if (root == NULL) return root;

    // 2. Update Height for current node
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // 3. Check Balance Factor
    int balance = getBalance(root);

    // 4. Perform Rotations if deletion caused imbalance
    // Left-Left Case
    if (balance > 1 && getBalance(root->left) >= 0) 
        return rightRotate(root);
    
    // Left-Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    
    // Right-Right Case
    if (balance < -1 && getBalance(root->right) <= 0) 
        return leftRotate(root);
    
    // Right-Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// ==========================================
// 4. TREE ANALYTICS & UTILITIES
// ==========================================

struct AVLNode* findMax(struct AVLNode* node) {
    struct AVLNode* current = node;
    while (current != NULL && current->right != NULL)
        current = current->right;
    return current;
}

// In an AVL tree, height is mathematically guaranteed to be O(log N). 
// We can just return root->height - 1 (since our leaves start at height 1).
int getTreeHeight(struct AVLNode* root) {
    if (root == NULL) return -1;
    return root->height - 1; 
}

int countNodes(struct AVLNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(struct AVLNode* root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return countLeaves(root->left) + countLeaves(root->right);
}

struct AVLNode* findLCA(struct AVLNode* root, int n1, int n2) {
    if (root == NULL) return NULL;
    if (root->data > n1 && root->data > n2) return findLCA(root->left, n1, n2);
    if (root->data < n1 && root->data < n2) return findLCA(root->right, n1, n2);
    return root;
}

int isBSTUtil(struct AVLNode* root, int min, int max) {
    if (root == NULL) return 1;
    if (root->data <= min || root->data >= max) return 0;
    return isBSTUtil(root->left, min, root->data) && 
           isBSTUtil(root->right, root->data, max);
}

int isBST(struct AVLNode* root) {
    return isBSTUtil(root, INT_MIN, INT_MAX);
}

// ==========================================
// 5. TRAVERSALS & MEMORY
// ==========================================

void inorderTraversal(struct AVLNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

void preorderTraversal(struct AVLNode* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(struct AVLNode* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->data);
    }
}

void freeTree(struct AVLNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ==========================================
// 6. DRIVER CODE
// ==========================================
int main() {
    struct AVLNode* root = NULL;

    printf("--- BUILDING THE AVL TREE ---\n");
    // Inserting sequential data to prove it self-balances
    // A standard BST would become a straight line of height 6.
    int values[] = {10, 20, 30, 40, 50, 25, 5};
    for (int i = 0; i < 7; i++) {
        root = insertAVL(root, values[i]);
        printf("Inserted %d\n", values[i]);
    }

    printf("\n--- TRAVERSALS ---\n");
    printf("In-order   : "); inorderTraversal(root); printf("\n"); // 5 10 20 25 30 40 50
    printf("Pre-order  : "); preorderTraversal(root); printf("\n"); // 30 20 10 5 25 40 50 

    printf("\n--- ANALYTICS ---\n");
    printf("Tree Height       : %d (Logarithmic, not linear!)\n", getTreeHeight(root));
    printf("Total Nodes       : %d\n", countNodes(root));
    printf("Total Leaf Nodes  : %d\n", countLeaves(root));
    
    struct AVLNode* minNode = findMin(root);
    if (minNode) printf("Minimum Value     : %d\n", minNode->data);
    
    struct AVLNode* maxNode = findMax(root);
    if (maxNode) printf("Maximum Value     : %d\n", maxNode->data);

    if (isBST(root)) {
        printf("Validation        : TRUE (Valid BST structure maintained)\n");
    }

    struct AVLNode* lca = findLCA(root, 5, 25);
    if (lca) printf("LCA of 5 & 25     : %d\n", lca->data);

    printf("\n--- DELETIONS (Testing Self-Balancing) ---\n");
    printf("Deleting 40...\n");
    root = deleteAVL(root, 40);
    
    printf("Deleting 50...\n");
    root = deleteAVL(root, 50);

    printf("Updated In-order  : "); 
    inorderTraversal(root); 
    printf("\n");
    
    printf("Updated Pre-order : "); 
    preorderTraversal(root); 
    printf("\n");

    freeTree(root);
    return 0;
}
