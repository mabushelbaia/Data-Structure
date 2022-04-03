#include <stdio.h>
#include<time.h>
#include <stdlib.h>

struct Node {
  int val;
  struct Node* next;
};

int isEmpty(struct Node* L){
  return L->next == NULL;
}
void printList(struct Node* L){ // prints only int-type Nodes
  if(!isEmpty(L)){
    struct Node* p = L->next;
    while(p != NULL){
      printf("%d ",p->val);
      p = p->next;
    }
    printf("\n");
    return;
  }
  else{
    printf("Empty List\n");
  }
}
int getSize(struct Node* L){ // Recursive
  if(L->next == NULL) return 0;
  return 1 + getSize(L->next);
}
int iGetSize(struct Node* L){ // Itterative
    int counter = 0;
    if(L->next == NULL) return 0;
    while(L->next != NULL){
      L = L->next;
      ++counter;
    }
    return counter;
}
void sortedList(struct Node* head,int size){
  struct Node* current;
  current = head;
  for(int i=0;i<size;++i){
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node*));
    current->next = temp;
    temp-> val = i*10 +rand() % 10;
    current = temp;
  }
}
void randomList(struct Node* head,int size){
  struct Node* current;
  current = head;
  for(int i=0;i<size;++i){
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node*));
    current->next = temp;
    temp-> val = rand() % 240;
    current = temp;
  }
}
int main(){
  struct Node* head;
  head = (struct Node*)malloc(sizeof(struct Node*));
  struct Node* current;
  current = head;
  for(int i=0;i<10;i++){
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node*));
    current->next = temp;
    temp-> val = rand() % 4071120;
    current = temp;
  }
  isEmpty(head)?printf("TRUE\n"):printf("FALSE\n");
  printList(head);
  printf("%d\t%d\n",getSize(head),iGetSize(head));
  return 0;
}
