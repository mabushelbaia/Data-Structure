#include <stdio.h>
#include<time.h>
#include <stdlib.h>

//"""DONT USE the insert of v2 with other insert versions"""
struct Node {
  int val;
  struct Node* next;
};
struct Node* curr;
int isEmpty(struct Node* L);
void printList(struct Node* L);
void insertV1(struct Node* head, struct Node* append);
void insertV2(struct Node* head, struct Node* append);
void insertV3(struct Node* head, struct Node* append);
void randomList(struct Node* head,int size);
void sortedList(struct Node* head,int size);
void insertSorted(struct Node* head, struct Node* append){
  if(head == NULL || append == NULL) return;
  if(head-> next == NULL) head-> next = append;
  if((int)head->next->val >= (int) append->val){
    insertV3(head,append);
    return;
  };
  struct Node* t;
  t = head-> next;
  append -> next = NULL;
  while(1){
    if(t->next == NULL) break;
    if(t->val <= append-> val && append-> val <= t->next->val) break;
    t =  t-> next;
  }
  append-> next = t-> next;
  t->next = append;
}


int main(){
  struct Node* head;
  head = (struct Node*) malloc(sizeof(struct Node*));
  head->next = NULL;

  sortedList(head,10);
  struct Node* start;
  struct Node* middle;
  struct Node* end;
  start = (struct Node*)malloc(sizeof(struct Node*));
  middle = (struct Node*)malloc(sizeof(struct Node*));
  end = (struct Node*)malloc(sizeof(struct Node*));
  start-> val = -10;
  middle-> val = 50;
  end-> val = 101;
  insertSorted(head,start);
  insertSorted(head,middle);
  insertSorted(head,end);
  printList(head);
  return 0;
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

void insertV1(struct Node* head, struct Node* append){ // O(N) insert at the end
  if(head == NULL || append == NULL) return;
  while(head->next != NULL) head = head->next;
  head->next = append;
  append->next = NULL;
}
void insertV2(struct Node* head, struct Node* append){ // O(1) : storing the current pos.
    if(head == NULL || append == NULL) return;
    curr->next = append;
    curr = append;
}
void insertV3(struct Node* head, struct Node* append){ //O(1) insert at front
  if(head == NULL || append == NULL) return;
  append-> next = head-> next;
  head-> next = append;
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
