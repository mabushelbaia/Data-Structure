#include <stdio.h>
#include<time.h>
#include <stdlib.h>

struct Node {
  int val;
  struct Node* next;
};
int isEmpty(struct Node* L);
void printList(struct Node* L);
void sortedList(struct Node* head, int size);
void deleteNode(struct Node* head, int key);
struct Node* findPrevious(struct Node* head, int key);
int main(){
  struct Node* head;
  head = (struct Node*)malloc(sizeof(struct Node*));
  head->next = NULL;
  sortedList(head,15); // appends n nodes to head each of value 10*i, i = 0,1,2 ... n-1
  deleteNode(head,100);
  /*
  0 10 20 30 40 50 60 70 80 90 100 110 120 130 140
  0 10 20 30 40 50 60 70 80 90 110 120 130 140
*/
  printList(head);
  return 0;
}



void deleteNode(struct Node* head, int key){
  if(isEmpty(head)) return;
  struct Node* previous = findPrevious(head, key);
  if(previous == NULL) return;
  struct Node* dNode = previous->next;
  previous->next = dNode-> next;
  free(dNode);
}





struct Node* findPrevious(struct Node* head, int key){
    struct Node* p = head;
    struct Node* q = p->next;
    while(q != NULL){
      if(q-> val == key) break;
      p = q;
      q = q->next;
    }
    return q==NULL?NULL: p;
}








void sortedList(struct Node* head,int size){
  struct Node* current;
  current = head;
  for(int i=0;i<size;++i){
    struct Node* temp;
    temp = (struct Node*)malloc(sizeof(struct Node*));
    current->next = temp;
    temp-> val = i*10;
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
