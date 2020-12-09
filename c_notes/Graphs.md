## Graphs

a graph consists of a set of vertices together with a set of edges each of which connects two vertices

Every edge has a non negative weight attached

 

could implement as a adjacency table

simply encode the weighted edges in a 2d array

![Screenshot 2020-12-08 at 13.35.58](/Users/danielchow/Desktop/Screenshot 2020-12-08 at 13.35.58.png)

where the values are the edge weights- if infinite then no connection



### Realloc version

```c
#define INITSIZE 8
#define SCALEFACTOR 2
#define NO_VERT -1
typedef unsigned int edge;


typedef struct graph
{
  edge ** adjmat;
  char** labels;
  int size;
  int capacity;
} graph;



graph* graph_init(void)
{
  int h,w;
  int i,j;
  graph* g =(graph*) ncalloc(sizeof(graph), 1);
  h=INITSIZE;
  w=h;
  g->capacity=h;
  g->adjmat=(edge**) n2dcalloc(h,w,sizeof(edge));
  g->labels=(char**) n2dcalloc(h,MAXLABEL+1,sizeof(char));
  for(j=0;j<h;j++)
  {
    for(i=0;i<h;i++)
    {
      g->adjmat[j][i]=INF;
    }
  }
  return g;
}


edge graph_getedgeweight(graph* g,int from, int to)
{
  if((g==NULL)||(from>=g->size)||(to >=g->size))
  {
    return INF;
  }
  return g->adjmat[from][to];
}


int graph_numVerts(graph* g)
{
   if(g==NULL){
      return 0;
   }
   return g->size;
}

bool graph_free(graph* g)
{
   n2dfree((void**)g->adjMat, g->capacity);
   n2dfree((void**)g->labels, g->capacity);
   free(g);
   return true;
}

int graph_addVert(graph* g, char* label)
{

   int i, j;
   if(g==NULL){
      return NO_VERT;
   }
   if(graph_getVertNum(g, label) != NO_VERT){
      return NO_VERT;
   }
   /* Resize */
   if(g->size >= g->capacity){
      g->adjMat = (edge**) n2drecalloc((void**)g->adjMat, g->capacity, g->capacity*SCALEFACTOR, g->capacity, g->capacity*SCALEFACTOR, sizeof(edge));
      g->labels = (char**) n2drecalloc((void**)g->labels, g->capacity, g->capacity*SCALEFACTOR, MAXLABEL+1, MAXLABEL+1,  1);
      for(j=0; j<g->capacity*SCALEFACTOR; j++){
         for(i=0; i<g->capacity*SCALEFACTOR; i++){
            if((i>=g->capacity)||(j>=g->capacity)){
               g->adjMat[j][i] = INF;
            }
         }
      }
      g->capacity = g->capacity*SCALEFACTOR;
   }
   strcpy(g->labels[g->size], label);
   g->size = g->size + 1;
   return g->size-1;
}

bool graph_addEdge(graph* g, int from, int to, edge w)
{
   if((g==NULL) || (g->size == 0)){
      return false;
   }
   if((from >= g->size) || (to >= g->size)){
      return false;
   }
   g->adjMat[from][to] = w;
   return true;
}

char* graph_getLabel(graph* g, int v)
{
   if((g==NULL) || (v >= graph_numVerts(g))){
      return NULL;
   }
   return g->labels[v];
}

void graph_tostring(graph* g, char* str)
{
   int f, t;
   char tmp[TMPSTR];
   str[0] = '\0';
   if(g==NULL){
      return;
   }
   for(f=0; f<g->size; f++){
      sprintf(tmp, "%s ", g->labels[f]);
      strcat(str, tmp);
   }
   for(f=0; f<g->size; f++){
      for(t=0; t<g->size; t++){
         if(_isedge(g,f,t)){
            sprintf(tmp, "%d->%d %d ",
                    f, t, g->adjMat[f][t]);
            strcat(str, tmp);
         }
      }
   }
}
```

### As linked list

![Screenshot 2020-12-08 at 13.53.12](/Users/danielchow/Desktop/Screenshot 2020-12-08 at 13.53.12.png)

Have all the nodes as a linked list going down-

then for each of the nodes have the edges connect from one node to another with their value being the weight of the edge

benefit of this over a array is that it saves a lot of space- problem is that it takes longer to search a linked list for the node you are looking for.



```c
typedef unsigned int edge;

struct vertex {
   char* label;
   struct vertex* nextv;
   void* firste;
   int num;
};
typedef struct vertex vertex;

struct edge {
   edge weight;
   vertex* v;
   struct edge* nexte;
};
typedef struct edge edgel;

struct graph {
   vertex* firstv;
   vertex* endv;
   int size;
};
typedef struct graph graph;
```

```c
graph* graph_init(void)
{
   graph* g = (graph*) ncalloc(sizeof(graph), 1);
   return g;
}

edge graph_getEdgeWeight(graph* g, int from, int to)
{
   int i;
   vertex *v;
   edgel* e;
   if((g==NULL) || (from >= g->size) || (to >= g->size)){
      return INF;
   }
   v = g->firstv;
   for(i=0; i<from; i++){
      v = v->nextv;
   }
   if((v==NULL) || (v->num != from)){
      return INF;
   }
   e = v->firste;
   while(e != NULL){
      if(e->v->num == to){
         return e->weight;
      }
      e = e->nexte;
   }
   return INF;
}

int graph_numVerts(graph* g)
{
    if(g==NULL||(g->endv==NULL)) {
        return 0;
    }
    return g->endv->num+1;
}

bool graph_free(graph* g)
{
   vertex* v, *tv;
   edgel* e, *te;
   if(g==NULL){
      return false;
   }
   v = g->firstv;
   while(v != NULL){
      e = v->firste;
      while(e != NULL){
         te = e->nexte;
         free(e);
         e = te;
      }
      tv = v->nextv;
      free(v->label);
      free(v);
      v = tv;
   }
   free(g);
   return true;
}

int graph_addVert(graph* g, char* label)
{

   vertex* n;
   if(g==NULL){
      return NO_VERT;
   }
   if(graph_getVertNum(g, label) != NO_VERT){
      return NO_VERT;
   }

   n = ncalloc(sizeof(vertex), 1);
   n->label = ncalloc(strlen(label)+1, 1);
   strcpy(n->label, label);
   if(g->firstv == NULL){
      /* Put in front */
      g->firstv = n;
   }
   else{
      /* Add to end */
      n->num = g->endv->num + 1;
      g->endv->nextv = n;
   }
   g->endv = n;
   g->size = g->size + 1;
   return g->size-1;
}

bool graph_addEdge(graph* g, int from, int to, edge w)
{
   vertex* f;
   vertex* t;
   int i;
   if((g==NULL) || (g->size == 0)){
      return false;
   }
   if((from >= g->size) || (to >= g->size)){
      return false;
   }
   f = g->firstv;
   for(i=0; i<from; i++){
      f = f->nextv;
   }
   t = g->firstv;
   for(i=0; i<to; i++){
      t = t->nextv;
   }
   return _addEdge(f, t, w);
}

bool _addEdge(vertex* f, vertex* t, edge w)
{

   edgel* e;
   edgel* b;
   if(f->firste ==NULL){
      /* 1st */
      e = ncalloc(sizeof(edgel), 1);
      f->firste = e;
      e->v = t;
      e->weight = w;
      return true;
   }
   e = f->firste;
   while(e != NULL){
      /* Already got it */
      if(e->v == t){
         return false;
      }
      b = e;
      e = e->nexte;
   }
   /* Add to end */
   b->nexte = ncalloc(sizeof(edgel), 1);
   b->nexte->v = t;
   b->nexte->weight = w;
   return true;
}

int graph_getVertNum(graph* g, char* label)
{
   int cnt = 0;
   vertex* v;
   v = g->firstv;
   while(v != NULL){
      if(strcmp(v->label, label)==0){
         return cnt;
      }
      v = v->nextv;
      cnt++;
   }
   return NO_VERT;
}

void graph_tostring(graph* g, char* str)
{
   vertex* v;
   edgel* e;
   char tmp[TMPSTR];
   str[0] = '\0';
   if(g==NULL){
      return;
   }
   v = g->firstv;
   while(v != NULL){
      sprintf(tmp, "%s ", v->label);
      strcat(str, tmp);
      v = v->nextv;
   }
   v = g->firstv;
   /* Edge Weights */
   while(v != NULL){
      e = v->firste;
      while(e != NULL){
         sprintf(tmp, "%d->%d %d ", v->num, e->v->num, e->weight);
         strcat(str, tmp);
         e = e->nexte;
      }
      v = v->nextv;
   }
}
```





## Travelling salesman

want to go to each node without really visiting nodes more than once

Finding the optimal path is NP-hard -this means there is not really a optimal path that is easy to find.

one greedy approach is just to go to the closest unvisited neighbour





## Dijkstra

shortest path through a graph from one vertex to another

