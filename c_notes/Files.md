## Files

```c
//read file
FILE *ipf
ifp=fopen(filename,"r");//opens file r for read
fclose(ifp);

//for reading blocks of info
fread(void *ptr, size,num_items,stream);

//for printing error messages use
fprintf(stderr,"Cannot do whatever %d",fname);
exit(EXIT_FAILURE);
```

