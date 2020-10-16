#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define UPPER 1000
#define LOWER 1
typedef int guess;
void indicator(guess ans);

guess rand_gen(void);

int main(void)
{
   srand(time(NULL));
   guess target;
   target = rand_gen();
   indicator(target);
}

guess rand_gen(void)
{
   return (guess) rand() % UPPER + LOWER;
}

void indicator(guess ans)
{
   guess attempt;
   int goes;
   goes =10;
   attempt=0;
   while(goes>0)
   {
      printf("have a go\n");
      scanf("%d", &attempt);
      printf("guess is %d\n", attempt);
      goes-=1;
      if(attempt==ans)
      {
         printf("right answer\n");
         return;
      }
      else if (attempt>ans)
      {
         printf("too high\n");
      }
      else
      {
         printf("too low\n");
      }
   }
   printf("failed\n");
}
