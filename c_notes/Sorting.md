## Sorting 





###MergeSort

divide and conquer - divide the array in two halfs mergesort each half and then merge the two halves together

```c
void merge(int arr[], int l, int m, int r)
{
    //this func merges two sub arrays of arr[]
    //first subarray is arr[l..m] l as in left not 1
    //second is arr[m+1...r]
    int i, j, k;
    //getting sizes of the two temp arrays
    int n1 = m - l + 1;
    int n2= r - m;
    //creating temp arrays
    int L[n1], R[n2];
    
   
    /* Copy data to temp arrays L[] and R[] */
    
    for (i = 0; i < n1; i++) 
    {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }
    
    
    i=0;
    j=0;
    k=l;
    while(i < n1 && j<n2)
    {
        if(L[i]<=R[j])
        {
            arr[k]= L[i];
            i++;
        }
        else
        {
            arr[k]= R[j];
            j++;
        }
        k++;
    }
    
    while(i<n1)
    {
        arr[k]= L[i];
        i++;
        k++;
    }
    while(j<n2)
    {
        arr[k]= R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l+r)/2;
        //sort right and left halves first
        merge_sort(arr,l,m);
        merge_sort(arr, m+1, r);
        
        merge(arr,l,m,r);
    }
}

```



## Quicksort

Quicksort is also divide and conquer- choose value in the array as a pivot key. this key is used to divide the array into two partitions. The left partition contains keys <= pviot key and the right side contains keys >pviot.

85421367 

LHS <=4

so we go from right hand side and see if each number is greater than 4, and if it isn't we swap it to the other side with a number that is greater than 4 if we find any

3542 1867

3142 5867

now we have a pivoted array

we can now quicksort each of the partitions- not necessarily halves this is dependant on the number you choose eg if we chose 4 as our pivot the two partitions would look like

31425   867

to choose the pivot number - cant compute the mean of the array as this is expensive in itself

instead we can just choose a random number and let the function handle itself

```c
void quicksort(int *a, int lft, int rt)
{
  int pivpoint;
  pivpoint =partition(a,lft,rt);
  if(lft<pivpoint)
  {
    quicksort(a,lft,pivpoint-1);
  }
  if(rt>pivpoint)
  {
    quicksort(a,pivpoint+1,rt);
  }
}


 int partition(int *a, int lft, int rt)
 {
   int piv;
   piv=a[1];
   while(lft<rt)
   {
     while(piv<a[rt]&&lft<rt)
     {
       /*find a value that is lower than pivot val*/
       rt--;
     }
     if(rt!=lft)
     {
       a[lft]=a[rt];
     }
     lft++;
   }
   while(piv>a[lft] &&lft<rt)
   {
     lft++;
   }
   if(rt!=lft)
   {
     a[rt]= a[lft];
     rt--;
   }

 }
	a[rt]=piv;
  return rt;
}
```

quicksort is actually built into c

we pass in a pointer to the start of the array, number of things in the array, size of the item of the array and then a compare function

```c
int intcompare(const void *a, const void *b)
{
  const int *ia=(const int *)a;
  const int *ib= (const int *)b;
  return *ia- *ib;
}


```

in a make file you can include a -D

this will add a #define whatever comes after the D 





## Radix sort

also known as bin sort

for number arrays -

basically you get all the numbers in the array look at the last digit in the number and bin them by that number so you'd create a bin of numbers ending in 1, bin of numbers ending in 2 and so on. 

then you would go down from 0- 9 left to right and make the array agin.

then do it with the second to last digit and repeat the process



however for this sort you would have to pad all the numbers in the array so they are all the same size

would struggle to do this for doubles and so on.





## String Searching

## Rabin Karp

we can treat a string like it is a number we saw this in hashing 

this helps us as we can compute the hash function of the string we are searching for. then for each substring we can compute the hash function of that substring and see if it is the same

and for each substring we only need to calculate one new characters contribution to the hash function at a time



there can be collisions just like in a hash function. 

## Boyer Moore



![Screenshot 2020-12-08 at 15.21.05](/Users/danielchow/Desktop/Screenshot 2020-12-08 at 15.21.05.png)

compare the R and G because there is no R in sting we slide 5 across, then compare G and S because sting starts with S we slide across 4 spaces - keep doing this until we find the substring.