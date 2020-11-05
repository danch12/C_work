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

can also do fgets to read into a string- then check the string then process the string using sscanf into digits or whatever.

fgets only gets one line at a time. 



also if array is too big we can define it as static so that it sits outside main and doesnt crash - this is a bad way of doing it really.

 Probably better to just do it the way i've been doing it already.