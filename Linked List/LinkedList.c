
#include <stdio.h>
#include <stdlib.h>


#include <stdio.h>
#include<time.h>
#include <stdlib.h>

struct Node {
  int val;
  struct Node* next;
};

void insert(struct Node* l, struct Node* temp){
    if(l != NULL && temp != NULL){
        temp-> next = l-> next;
        l->next = temp;
        return;
    }
}
int mian(){
    struct Node* head = NULL;
    head = (struct Node*) malloc(sizeof(struct Node));
    for(int i=0;i<20;i++){
        struct Node* temp = NULL;
        temp = (struct Node*)malloc(sizeof(struct Node));
        temp-> val = rand();
        insert(head,temp);
    }
    return 0;
}

