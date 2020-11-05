## Search

### Binary search

```c
int binary_it(int k,int *a, int lf, int rt)
{
  int m;
  while(lf<=rt)
  {
    m= (lf+rt)/2;
    if(k==a[m])
    {
      return m;
    }
    else
    {
      if(k>a[m])
      {
        lf= m+1;
      }
      else
      {
        rt=m-1;
      }
    }
  }
  return -1;
}
```

```c
int binary_rec(int k,int *a, int lf, int rt)
{
  int m;
  
  if(lf>rt)
  {
    return -1;
  }
  m=(lf+rt)/2;
  if(k==a[m])
  {
    return m;
  }
 	if(k>a[m])
  {
    return binary_rec(k,a,m+1,rt);
  }
  if(k<a[m])
  {
    return binary_rec(k,a,lf,rt-1);
  }
}

```

### Interpolation search

when we look for a word in a dictionary we dont start in the middle - we make educated guess as to where to start based on letters

i=(k-l[0])/(l[n-1]-l[0])*n

when searching for 15

```c
0  4  9  10  12  15  20
i= (15-0)/(20-0)*8
we looking for 15 so k =15
l is our array is l[0] is 0
and l[n-1] is 20
array is 8 long so times by 8
```

```c
int interp(int k, int *a, int lf, int r)
{
  int m;
  double md;
  while(lf<=r)
  {
    md=((double)(k-a[lf])/(double)(a[r]-		       a[lf])* (double)(r-lf))+(double)(lf);
    m=0.5+md;
    if(k==a[m])
    {
      return m;
    }
    else
    {
      if(k>a[m])
      {
        lf=m+1;
      }
      else
      {
        r=m-1;
      }
    }
  }
  return -1;
}
```

