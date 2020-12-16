#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void on_error(const char* s);
void* ncalloc(int n, size_t size);
void** n2dcalloc(int h, int w, size_t size);
void** n2drecalloc(void** p, int oh, int nh, int ow, int nw, size_t szelem);
void n2dfree(void**p, int h);
void* nrecalloc(void* p, int oldbytes, int newbytes);
void* nremalloc(void* p, int bytes);
void* nfopen(char* fname, char* mode);