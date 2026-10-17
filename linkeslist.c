#include <stdio.h>
#include <stdlib.h>

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


int main(){

  scanf("%d",&n);

}
