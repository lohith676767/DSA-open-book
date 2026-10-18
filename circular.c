#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node{
  int data;
  struct Node* next;
};

struct Node* createNode(int data){
  struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

struct Node* insertStart(struct Node* head,int data){
  struct Node* newNode = createNode(data);
  if(head==NULL){
    newNode->next = newNode;
    return newNode;
  }
  struct Node* temp = head;
  while(temp->next!=head){
   temp = temp->next;}
  temp->next = newNode;
  newNode->next = head;
  return newNode;
}

struct Node* insertEnd(struct Node* head,int data){
  struct Node* newNode = createNode(data);
  if(head==NULL){
    newNode->next = newNode;
    return newNode;
  }
  struct Node* temp = head;
  while(temp->next!=head){
   temp = temp->next;}
  temp->next = newNode;
  newNode->next = head;
  return head;
}

struct Node* deleteStart(struct Node* head){
  if(head==NULL){return NULL;}
  if(head->next == head){
        free(head);
        return NULL;
  }
  struct Node* temp = head;
  while(temp->next !=head){
    temp = temp->next;
  }
  struct Node* toDelete = head;
  struct Node* newHead = head->next;
  temp->next = newHead;
  free(toDelete);
  return newHead;
}

struct Node* deleteEnd(struct Node* head){
    if(head == NULL){return NULL;}
    if(head->next == head){
        free(head);
        return NULL;
    }
    struct Node* temp = head;
    while(temp->next->next != head){
        temp = temp->next;
    }
    struct Node* toDelete = temp->next;
    temp->next = head;
    free(toDelete);
    return head;
}

void printList(struct Node* head){
    if(head == NULL){
        printf("Empty list\n");
        return;
    }
    struct Node* temp = head;
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while(temp != head);
    
    printf("(head)\n");
}

bool search(struct Node* head, int data){
    if(head == NULL){
        return false;
    }
    struct Node* temp = head;
    do {
        if(temp->data == data){
            return true;
        }
        temp = temp->next;
    } while(temp != head);
    
    return false;
}
