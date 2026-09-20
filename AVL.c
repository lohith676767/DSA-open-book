#include <stdio.h>
#include <stdlib.h>

// 1. Upgraded Node Structure
struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height; // AVL specific: tracks node depth for balancing
};

// --- Utility Functions ---

// Get height safely (handles NULL nodes)
int getHeight(struct AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Get maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Calculate the balance factor of a node
int getBalance(struct AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Create a new AVL node (default height is 1)
struct AVLNode* createNode(int data) {
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // New node is initially added at leaf level
    return node;
}

// --- Rotation Mechanics ---

// Right Rotation (Fixes Left-Left case)
struct AVLNode* rightRotate(struct AVLNode* y) {
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights (must update y first, as it is now below x)
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root of this subtree
    return x;
}

// Left Rotation (Fixes Right-Right case)
struct AVLNode* leftRotate(struct AVLNode* x) {
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights (must update x first)
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return new root of this subtree
    return y;
}

// --- AVL Insertion ---

struct AVLNode* insertAVL(struct AVLNode* node, int data) {
    // 1. Standard BST Insertion
    if (node == NULL) {
        return createNode(data);
    }

    if (data < node->data) {
        node->left = insertAVL(node->left, data);
    } else if (data > node->data) {
        node->right = insertAVL(node->right, data);
    } else {
        return node; // Duplicate keys are not allowed in this implementation
    }

    // 2. Update the height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get the balance factor to check if it became unbalanced
    int balance = getBalance(node);

    // 4. If unbalanced, trigger one of the 4 rotation cases

    // Case 1: Left Left Case
    // (Left is heavy, and the new data went down the left path of the left child)
    if (balance > 1 && data < node->left->data) {
        return rightRotate(node);
    }

    // Case 2: Right Right Case
    // (Right is heavy, and the new data went down the right path of the right child)
    if (balance < -1 && data > node->right->data) {
        return leftRotate(node);
    }

    // Case 3: Left Right Case
    // (Left is heavy, but the new data went down the RIGHT path of the left child)
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left); // Straighten the kink first
        return rightRotate(node);            // Then rotate right
    }

    // Case 4: Right Left Case
    // (Right is heavy, but the new data went down the LEFT path of the right child)
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right); // Straighten the kink first
        return leftRotate(node);                // Then rotate left
    }

    // Return the (unchanged) node pointer if perfectly balanced
    return node;
}

// Pre-order traversal to verify the tree structure (Root, Left, Right)
void preOrder(struct AVLNode* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main() {
    struct AVLNode* root = NULL;

    /* Constructing tree by inserting sorted data. 
       In a standard BST, this would create a terrible straight line.
       In an AVL tree, it will automatically rotate to stay perfectly balanced. */
    root = insertAVL(root, 10);
    root = insertAVL(root, 20);
    root = insertAVL(root, 30); // Triggers RR Case (Left Rotation)
    root = insertAVL(root, 40);
    root = insertAVL(root, 50); // Triggers RR Case (Left Rotation)
    root = insertAVL(root, 25); // Triggers RL Case (Right-Left Rotation)

    /* The constructed AVL Tree should look like this:
            30
           /  \
         20    40
        /  \     \
       10  25    50
    */

    printf("Pre-order traversal of the constructed AVL tree is: \n");
    preOrder(root); 
    // Expected output: 30 20 10 25 40 50
    printf("\n");

    return 0;
}
