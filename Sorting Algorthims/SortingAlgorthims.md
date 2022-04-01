# Bubble Sort

**illustation**: https://youtu.be/nmhjrI-aW5o

**Complexity**: O(n^2)

Sorting Elements by repeatedly swapping the adjacent elements if they are in wrong order.

**Code:**

```cpp
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
void BubbleSort(int arr[]){
    int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0;i<n-1;++i){
        for(int j-0;j<n-i-1;++j){
            if(arr[j]<arr[j+1]){
                swap(&arr[j+1],&arr[j])
            }
        }
    }
}
```

# Merge Sort

**illustation:** https://youtu.be/JSceec-wEyw

**Algorithmic Paradigm:** Divide and Conquer

**Complexity**: O(n*log(n))

Sorting arrays on different machines. Merge Sort is a recursive algorithm and time complexity can be expressed as following recurrence relation.   

> T(n) = 2T(n/2) + θ(n)

**CODE:**

```cpp
void merge(int arr[], int p, int q, int r) {

    // Create L ← A[p..q] and M ← A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    int L[n1], M[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k++] = L[i++];
            continue;
        }
        arr[k++] = M[j++];
    }

    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = M[j++];

}
void mergeSort(int arr[],int l,int r){
    if(l < r){
      int m = l + (r-l)/2;
      mergeSort(arr,l,m);
      mergeSort(arr,m+1,r);
      merge(arr,l,m,r);
    }
}
int main(){
  int arr[] = {4,47,13,15,20,70,21,17};
  int n = sizeof(arr)/sizeof(arr[0]);
  mergeSort(arr,0,n-1);
  for(int x: arr){
    std::cout <<x<<" ";
  }
  return 0;
}
```
