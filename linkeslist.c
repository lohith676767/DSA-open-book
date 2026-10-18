#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node*{
    int data;
    struct Node* next;
}

struct Node* createNode(int data){

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode ->data = data;
    newNode ->next = NULL;
    return newNode;

}

struct Node* insertStart(struct Node* head,int data){

    struct Node* temp = head;
    struct Node* startNode = createNode(data);
    if(temp==NULL){
        return startNode;
    }
    startNode->next = temp;
    return startNode;


}

struct Node* insertEnd(struct Node* head,int data){

    struct Node* temp = head;
    struct Node* endNode = createNode(data);
    if(temp==NULL){
        return endNode;
    }
    while(temp->next != NULL){
        temp = temp->next;
    }
    
    temp->next = endNode; 
    return head;

}

struct Node* insertPos(struct Node* head,int data,int pos){
    int counter = 0;
    struct Node* temp = head;
        if(pos==0){
        return insertStart(head,data);
    }
    struct Node* posNode = createNode(data);
    if(temp==NULL){
        return posNode;
    }
    while((temp->next != NULL)&&(counter<pos-1)){
        temp = temp->next;
        counter++;
    }
    
    posNode->next = temp->next ;
    temp->next = posNode; 
    return head;

}

struct Node* deleteStart(struct Node* head){

     if(head==NULL){
       return NULL;
     }
     struct Node* temp = head;
     head=head->next;
     free(temp);
     return head;
  
}

struct Node* deleteEnd(struct Node* head){

    if(head==NULL){
       return NULL;
     }
    if(head->next == NULL){
       free(head);
       return NULL;
    }
    struct Node* temp = head;
    while(temp->next->next !=NULL){
       temp=temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    return head;

}

struct Node* deletePos(struct Node* head,int pos){
    if(head == NULL){
        return NULL;
    }
    if(pos == 0){
        return deleteStart(head);
    }
    int counter = 0;
    struct Node* temp = head;
    while(temp->next!=NULL && counter<pos-1){
        temp=temp->next;
        counter++;
    }
    if(temp->next == NULL){
        return head;
    }

    struct Node* nodeToDelete = temp->next;
    temp->next = nodeToDelete->next;
    free(nodeToDelete);
    return head;
}

bool search(struct Node* head,int data){
   struct Node* temp = head;
   while(temp!=NULL){
     if(temp->data == data){
       return true;
     }
     temp = temp->next;
   }
   return false;
}

struct Node* reverseList(struct Node* head) {
    struct Node* prev = NULL;
    struct Node* current = head;
    struct Node* next = NULL
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}   

int main(){

  return 0;

}
