#include <bits/stdc++.h>
using namespace std;


int main(){
  int arr[] = {0,7,15,21,30,70,90,99};
  int target = 30;
  int index = -1;
  int n = sizeof(arr)/sizeof(arr[0]);
  int left = 0,right = n - 1;
  while(left <= right){
    int middle = left + (right - left)/2; // to avoid overflow
    int val = arr[middle];
    if(val == target){
        index = middle;
        break;
    }
    else if (val < target){
      left = middle + 1;
    }
    else{
      right = middle - 1;
    }
  }
  std::cout <<"Index: "<<index<< '\n';
  return 0;
}
