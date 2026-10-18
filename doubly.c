#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {

  int data;
  struct Node* prev;
  struct Node* next;

};

struct Node* createNode(int data){
  struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
  newNode->data = data;
  newNode->next=NULL;
  newNode->prev=NULL;
  return newNode;
}

struct Node* insertStart(struct Node* head,int data){
  if(head==NULL){
   return createNode(data);
  }
  struct Node* firstNode = createNode(data);
  head->prev = firstNode;
  firstNode->next = head;
  return firstNode;
}

struct Node* insertEnd(struct Node* head,int data){
  if(head==NULL){
   return createNode(data);
  }
  struct Node* temp = head;
  while(temp->next!=NULL){
   temp = temp->next;
  }
  struct Node* endNode = createNode(data);
  temp->next = endNode;
  endNode->prev = temp;
  return head;
}

struct Node* insertPos(struct Node* head,int data,int pos){
  if(head==NULL){
    return createNode(data);
  }
  if(pos==0){
    return insertStart(head,data);
  }
  struct Node* temp = head;
  int counter=0;
  while(temp->next !=NULL && counter<pos-1){
    temp=temp->next;
    counter++;
  }
  struct Node* toInsert = createNode(data);
  
  toInsert->next = temp->next;
  toInsert->prev = temp;
  temp->next=toInsert;
  if(toInsert->next!=NULL){
  toInsert->next->prev=toInsert;}
  return head;
}

struct Node* deleteStart(struct Node* head){
  if(head==NULL){
    return NULL;
  }
  struct Node* temp = head;
  head=head->next;
  if(head!=NULL){head->prev = NULL;}
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
  while(temp->next!=NULL){
   temp=temp->next;
  }
  temp->prev->next = NULL;
  free(temp);
  return head;
}
  
struct Node* deletePos(struct Node* head,int pos){
  int counter=0;
  if(head==NULL){
    return NULL;
  }
  if(pos==0){
   return deleteStart(head);}
  struct Node* temp = head;
  while(temp!=NULL && counter<pos){
   temp=temp->next;
   counter++;
  }
  if(temp == NULL){return head;}
  if(temp->next!=NULL){
  temp->next->prev = temp->prev;}
  if(temp->prev!=NULL){
  temp->prev->next = temp->next;}
  free(temp);
  return head;
}

bool isthere(struct Node* head,int data){
  struct Node* temp = head;
  while(temp!=NULL){
   if(temp->data == data){return true;}
   temp = temp->next;
  }
  return false;
}

int min(struct Node* head){
  if(head==NULL){return -1;}
  struct Node* temp = head;
  int min = head->data;
  int counter=0;
  int index=0;
  while(temp!=NULL){
   if(temp->data <min){
     min=temp->data;
     index = counter;
   }
   temp = temp->next;
   counter++;
  }
  printf("Min value is %d\n",min);
  return index;
}

struct Node* deleteMin(struct Node* head){
  return deletePos(head,min(head));
}

void printfwd(struct Node* head){
  if(head==NULL){
    printf("\nEmpty list\n");
    return;
  }
  struct Node* temp = head;
  printf("NULL->");
  while(temp->next !=NULL){
    printf("%d ->",temp->data);
    temp=temp->next;
  }
  printf("%d->NULL\n",temp->data);
}

int main(){
  printf("number of Entries: ");
  int n;
  scanf("%d",&n);
  int data;
  printf("Entry 1 : ");
  scanf("%d",&data);
  struct Node* lift = createNode(data);
  for(int i=0;i<n-1;i++){
    printf("Entry %d : ",i+2);
    scanf("%d",&data);
    if(!isthere(lift,data)){
      insertEnd(lift,data);
    }else{printf("duplicate not added\n");}
  }
  printfwd(lift);
}
