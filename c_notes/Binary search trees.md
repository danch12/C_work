# Binary search trees

These trees are usually sorted - on the left side they are smaller and on the right side they are larger.

to insert a value into the tree

```c
dataframe* insert(dataframe* t, datatype d)
{
  /* if tree is empty return a new frame*/
  if(t==NULL)
  {
    f=ncalloc(sizeof(dataframe),1);
    f->d=d;
    return f;
  }
  if(d< t->d)
  {
    t->left = insert(t->left,d);
  }
  else if(d>t->d)
  {
    t->right = insert(t->right, d);
  }
  /*return top*/
  return t;
}
```



to search for value in tree

```c
bool isin(dataframe* t, datatype d)
{
  if(t==NULL)
  {
    return false;
  }
  if(t->d==d)
  {
    return true;
  }
  if(d<t->d)
  {
    return isin(t->left,d);
  }
 	else
  {
    return isin(t->right, d);
  }
  return false;
}
```

level order traversal

to do this we can use a queue- each step we take the parent out of the queue and add in the child nodes



```c
void bst_printlevel(bst* b)
{
  datatype n;
  queue* q;
  if(b==NULL)||()
}
```

