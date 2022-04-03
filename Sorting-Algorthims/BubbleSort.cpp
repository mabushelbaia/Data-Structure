#include <bits/stdc++.h>

void swap(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}


int main(){
    int arr[] = {11,12,-3,15,90,30,40,100,-10,0};
    int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0;i< n-1;i++){
        for(int j=0;j< n-i-1;j++){
            if(arr[j]<arr[j+1]){
                swap(&arr[j],&arr[j+1]);
            }
        }
    }
    for (int x : arr) {
      cout << x <<" ";
    }
    return 0;
}
