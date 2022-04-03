#include <stdio.h>
#include<time.h>
#include <stdlib.h>
/* Problem: delete all odd valued node */

struct Node {
  int val;
  struct Node* next;
};


int isEmpty(struct Node* L);
void printList(struct Node* L);
void randomList(struct Node* head, int size);
// void deleteNode(struct Node* head, int key);
// struct Node* findPrevious(struct Node* head, int key);


int main(){
  struct Node* head;
  head = (struct Node*)malloc(sizeof(struct Node*));
  head->next = NULL;
  randomList(head,10); // appends 10 random valued nodes to head.
  printList(head);
  struct Node* pre = head;
  struct Node* cur = head-> next;
  while(cur!= NULL){
    if((cur->val)%2){
      pre->next = cur->next;
      cur = pre-> next;
      continue;
    }
    pre = cur;
    cur = cur->next;
  }
  printList(head);
  return 0;
}


void randomList(struct Node* head,int size){
  struct Node* current;
  current = head;
  for(int i=0;i<size;++i){
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node*));
    current->next = temp;
    temp-> val = rand() % 100;
    current = temp;
  }
}


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
