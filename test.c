#include <stdio.h>
#include<stdlib.h>

void merge(int arr[], int l, int m, int r);

void merge_sort(int arr[], int l, int r);
void printarr(int arr[],int size);

int main(void)
{
    int test_arr[]={3,7,10,40,3,20};
    printarr(test_arr,6);
    merge_sort(test_arr,0,6);
    printarr(test_arr,6);
}


void merge(int arr[], int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l +1;
  int n2 = r-m;
  int L[n1], R[n2];

  for (i=0; i<n1;i++)
  {
    L[i]= arr[l+i];
  }
  for (i=0;i<n2;i++)
  {
    R[i]= arr[m+i+1];
  }

  k = j =0;
  i=l;

  while(j<n1&& k<n2)
  {
    if (L[j]<=R[k])
    {
      arr[i]= L[j];
      j++;
    }
    else
    {
      arr[i]=R[k];
      k++;
    }
    i++;
  }

  while(j<n1)
  {
    arr[i]= L[j];
    i++;
    j++;
  }
  while(k<n2)
  {
    arr[i]=R[k];
    i++;
    k++;
  }

}


void merge_sort(int arr[], int l, int r)
{
  if(l<r)
  {
    int m =(l+r)/2;
    merge_sort(arr,l,m);
    merge_sort(arr,m+1,r);
    merge(arr,l,m,r);
  }
}

void printarr(int arr[],int size)
{
  int i;
  for(i=0; i<size;i++)
  {
    printf("%d\n", arr[i]);
  }
}
