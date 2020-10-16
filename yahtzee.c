#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define MAXTHROW 6
#define LOWER 1
#define NUMDICE 5
#define TESTS 10000000

typedef enum bool{false,true} bool;


/*const is used when you pass an array or something
  but you dont want the array to be changed
  https://softwareengineering.stackexchange.com/questions/204500/when-and-for-what-purposes-should-the-const-keyword-be-used-in-c-for-variables
  */

/*Fill the array d with random numbers 1..6*/
void randomthrow(int d[NUMDICE]);
/*Decide if the number n occurs anywhere in the histogram h*/
bool histo_has(const int h[MAXTHROW],const int n);
/*Compute a histogram, given a dice hand*/
void makehist(const int d[NUMDICE],int h[MAXTHROW]);
/*Check that the histograms h1 & h2 are the same*/
bool hists_same(const int h1[MAXTHROW],const int h2[MAXTHROW]);
/*Does this hand have 2 lots of one number and 3 lots of another*/
bool isfullhouse(const int d[NUMDICE]);
/*Does this hand have 4 lots of one number and 1 of another ?*/
bool is4ofakind(const int d[NUMDICE]);
/*Do some testing of the functions required*/
void test(void);

int main(void)
{
   int dice[NUMDICE];
   int k4,fh,i;
   k4=0;
   fh=0;
   test();

   for(i=0;i<TESTS;i++)
   {
      randomthrow(dice);
      if(isfullhouse(dice))
      {
         fh++;
      }
      if(is4ofakind(dice))
      {
         k4++;
      }
   }
   printf("FH=%.2f%% 4oK=%.2f%%\n", (double)fh*100.0/(double)TESTS,
                                    (double)k4*100.0/(double)TESTS);
}


void test(void)
{
   int i, j;
   int h1[MAXTHROW] = {0,0,0,0,0,5}; /* 5−of−a−kind */
   int h2[MAXTHROW] = {0,0,5,0,0,0}; /* 5−of−a−kind */
   int h3[MAXTHROW] = {0,2,0,0,3,0}; /* Full−House */
   int h4[MAXTHROW] = {2,0,3,0,0,0}; /* Full−House */
   int h5[MAXTHROW] = {3,2,0,0,0,0}; /* Histo of d1 */
   int h6[MAXTHROW] = {0,0,0,2,3,0}; /* Histo of d2 */
   int h7[MAXTHROW] = {4,1,0,0,0,0}; /* Histo of d3 */
   int h8[MAXTHROW] = {0,0,0,0,1,4}; /* Histo of d4 */
   int h9[MAXTHROW]; /* Temp */
   int d1[NUMDICE] = {1,1,1,2,2}; /* Full House */
   int d2[NUMDICE] = {5,4,5,4,5}; /* Full House */
   int d3[NUMDICE] = {2,1,1,1,1}; /* 4−of−a−kind */
   int d4[NUMDICE] = {6,6,6,6,5}; /* 4−of−a−kind */
   int d5[NUMDICE] = {6,6,6,6,5}; /* Temp */

   /* Tricky to test a random throw ... */
   for(i=0; i<100; i++)
   {
      randomthrow(d5);
      for(j=0; j<NUMDICE; j++)
      {
         assert((d5[j] >= 1) && (d5[j] <= MAXTHROW));
      }
      makehist(d5,h9);
      for(j=0; j<MAXTHROW; j++)
      {
         assert((h9[j] >= 0) && (h9[j] < NUMDICE));
      }
   }

   assert(hists_same(h1,h1)==true);
   assert(hists_same(h5,h5)==true);
   assert(hists_same(h1,h2)==false);
   assert(hists_same(h4,h5)==false);
   makehist(d1,h9); assert(hists_same(h9,h5));
   makehist(d2,h9); assert(hists_same(h9,h6));
   makehist(d3,h9); assert(hists_same(h9,h7));
   makehist(d4,h9); assert(hists_same(h9,h8));
   assert(histo_has(h1,5)==true);
   assert(histo_has(h1,0)==true);
   assert(histo_has(h1,2)==false);
   assert(histo_has(h2,5)==true);
   assert(histo_has(h2,0)==true);
   assert(histo_has(h2,2)==false);
   assert(histo_has(h3,3)==true);
   assert(histo_has(h3,2)==true);
   assert(histo_has(h3,1)==false);
   assert(histo_has(h4,3)==true);
   assert(histo_has(h4,2)==true);
   assert(histo_has(h4,1)==false);
   assert(isfullhouse(d1)==true);
   assert(isfullhouse(d2)==true);
   assert(is4ofakind(d1)==false);
   assert(is4ofakind(d2)==false);
   assert(isfullhouse(d3)==false);
   assert(isfullhouse(d4)==false);
   assert(is4ofakind(d3)==true);
   assert(is4ofakind(d4)==true);
}



/*Fill the array d with random numbers 1..6*/
void randomthrow(int d[NUMDICE])
{
   int i, num;

   for(i=0;i<NUMDICE;i++)
   {
      num =rand()% MAXTHROW +LOWER;
      d[i]= num;
   }
}

bool histo_has(const int h[MAXTHROW],const int n)
{
   int i;
   for(i=0;i<MAXTHROW;i++)
   {
      if(h[i]==n)
      {
         return true;
      }
   }
   return false;
}

void makehist(const int d[NUMDICE],int h[MAXTHROW])
{

   int i;

   for(i=0;i<MAXTHROW;i++)
   {
      h[i]=0;
   }

   for(i=0;i<NUMDICE;i++)
   {
      h[d[i]-1]++;
   }
}

bool hists_same(const int h1[MAXTHROW],const int h2[MAXTHROW])
{
   int i;
   for(i=0;i<MAXTHROW;i++)
   {
      if(h1[i]!=h2[i])
      {
         return false;
      }
   }
   return true;
}


bool isfullhouse(const int d[NUMDICE])
{
   int i,count3,count2;
   int hist[MAXTHROW];
   makehist(d,hist);
   count3=0;
   count2=0;
   for(i=0;i<MAXTHROW;i++)
   {
      if(hist[i]==3)
      {
         count3=1;
      }
      if(hist[i]==2)
      {
         count2=1;
      }
   }
   if(count3==1 && count2==1)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool is4ofakind(const int d[NUMDICE])
{
   int i;
   int hist[MAXTHROW];
   makehist(d,hist);
   for(i=0;i<MAXTHROW;i++)
   {
      if(hist[i]==4)
      {
         return true;
      }
   }
   return false;
}
