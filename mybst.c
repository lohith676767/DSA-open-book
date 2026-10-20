#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node{
  int data;
  struct Node* left;
  struct Node* right;
};

struct Node* createNode(int data){
  struct Node* root = (struct Node*)malloc(sizeof(struct Node));
  root->data= data;
  root->left= NULL;
  root->right= NULL;
  return root;
}

struct Node* insert(struct Node* root,int data){
  
  if( root==NULL ){ return createNode(data); }
  if( data<root->data ){ root->left = insert(root->left,data); }
  else if( data>root->data ){ root->right = insert(root->right,data); }
  return root;
  
}

struct Node* search(struct Node* root,int data){
  if ( root==NULL||root->data==data ){return root;}
  if ( data<root->data ){ return search( root->left,data );}
  else { return search( root->right,data );}
}
  
struct Node* findMin(struct Node* root){
  struct Node* current = root;
  while(current!=NULL && current->left != NULL){
    current = (current->left);
  }
  return current;
}

struct Node* findMax(struct Node* root){
  struct Node* current = root;
  while(current!=NULL && current->right != NULL){
    current = (current->right);
  }
  return current;
}

struct Node* deleteNode(struct Node* root,int data){

  if( root == NULL){return root;}
  if( data < root->data ){ root -> left = deleteNode(root->left,data);}
  else if( data > root->data ){ root ->right = deleteNode(root->right,data);}
  else{
    if(root->left==NULL){
      struct Node* temp = root->right;
      free(root);
      return temp;
    }else if(root->right == NULL){
      struct Node* temp = root->left;
      free(root);
      return temp;
    }

    struct Node* temp = findMin(root->right);
    root->data = temp->data;
    root->right= deleteNode(root->right,temp->data);
  }
  return root;
  
}

int findHeight(struct Node* root){
  if(root==NULL){return -1;}
  int leftHeight = findHeight(root->left);
  int rightHeight = findHeight(root->right);
  return(leftHeight>rightHeight ? leftHeight+1 : rightHeight+1);
}

int countNodes(struct Node* root){
  if(root==NULL){return 0;}
  return 1+countNodes(root->left)+countNodes(root->right);
}

int countLeaves(struct Node* root){
  if(root==NULL){return 0;}
  if(root->left==NULL&&root->right==NULL){return 1;}
  return countLeaves(root->right)+countLeaves(root->left);
}

struct Node* finLCA(struct Node* root,int n1,int n2){

  if(root==NULL){return root;}
  if(root->data<n1 && root->data<n2){return findLCA(root->right,n1,n2);}
  if(root->data>n1 && root->data>n2){return findLCA(root->left,n1,n2);}
  return root;

}

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

int main(){
  return 0 ;
}
