#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Stack{
   int data;
   struct Stack* next;
};

bool is_empty(struct Stack* top){return top==NULL;}

void push(struct Stack** top,int data){
       struct Stack* newNode = (struct Stack*)malloc(sizeof(struct Stack));
       newNode->data = data;
       newNode->next = *top;
       *top = newNode;
}

int pop(struct Stack** top){
       if(is_empty(*top))return -1;
       struct Stack* temp = *top;
       int popped = temp->data;
       *top = (*top)->next;
       free(temp);
       return popped;
}

int peek(struct Stack* top){
        if(is_empty(top))return -1;
        return top->data;
}

void display(struct Stack* top){
        if(is_empty(top))printf("Stack is empty\n");
        struct Stack* temp = top;
        while(temp->next!=NULL){
               printf("%d ->",temp->data);
               temp = temp->next;
        }
        printf("NULL\n");
}

int main(){

        struct Stack* top = NULL;
        push(&top,10);
        push(&top,20);
        push(&top,30);
        push(&top,40);
        push(&top,50);
        push(&top,60);
        display(top);
        pop(&top);
        display(top);

}
