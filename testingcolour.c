#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enum neillcol {black=30, red, green, yellow, blue, magenta, cyan, white};
typedef enum neillcol neillcol;

void neillclrscrn(void);
void neillcursorhome(void);
void neillfgcol(const neillcol c);
void neillbusywait(const double secs);


int main(void)
{
   int i;
   char* test_str="@@**--@\n@@**--@";
   printf("\033[40m");
   for(i=0;test_str[i]!='\0';i++)
   {

      if(test_str[i]=='@')
      {
         neillfgcol(green);
      }
      if(test_str[i]=='*')
      {
         neillfgcol(red);
      }
      printf("%c",test_str[i]);
   }
   neillclrscrn();
   neillcursorhome();
   printf("\n");
}




/* Issue ANSI Codes to clear screen */
void neillclrscrn(void)
{
   printf("\033[2J");
}

/* Issue ANSI Codes to move cursor home */
void neillcursorhome(void)
{
   printf("\033[H");
}

/* Issue ANSI Codes change foreground colour */
void neillfgcol(const neillcol c)
{
   printf("\033[%dm", c);
}

/* May the coding Gods forgive this code
   You really want to give time to other
   processes, rather than st in a busy loop */
void neillbusywait(const double secs)
{
   clock_t t2;
   const clock_t t1 = clock();
   do{
      t2 = clock();
   }while((t2-t1) < (clock_t)((double)CLOCKS_PER_SEC*secs));
}
