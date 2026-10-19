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
  
  if(root==NULL){
    return createNode(data);
  }
  if(data<root->data){
    root->left = insert(root->left,data);
  }else if(data>root->data){
    root->right = insert(root->right,data);
  }
  return root;
  
}

struct Node* search(struct Node* root,int data){
  if(root==NULL||root->data==data){return root;}
  if(data<root->data){return search(root->left,data);}
  else{return search(root->right,data);}
}


