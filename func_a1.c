#include "common.h"
/*
   Returns a Boolean, depending upon whether the
   string s1 is composed entirely from characters that
   occur in s2 or not.
*/
char* strnodupes(const char* s);

int main(void)
{

   char* s2;

   s2 = strnodupes("Hello World!");
   assert(strcmp("Helo Wrd!", s2)==0);
   free(s2);
   s2 = strnodupes("a-aaaaaAAAAAAAAaaaaaaaa");
   assert(strcmp("a-A", s2)==0);
   free(s2);
   s2 = strnodupes("HHiHHiiidiHiHiedHidiHi");
   assert(strcmp("Hide", s2)==0);
   free(s2);
   s2 = strnodupes("Subdermatoglyphic");
   assert(strcmp("Subdermatoglyphic", s2)==0);
   free(s2);

   return 0;
}


bool struses(const char* s1, const char* s2)
{
   int size_s1,size_s2,i,j;
   bool state;
   size_s1=strlen(s1);
   size_s2=strlen(s2);
   for(i=0;i<size_s1;i++)
   {
      state=false;
      for(j=0;j<size_s2;j++)
      {
         if(s1[i]==s2[j])
         {
            state=true;
         }
      }
      if(state==false)
      {
         return false;
      }
   }
   return true;
}


char* strnodupes(const char* s)
{
   char* str;
   int len, i, j, k, cnt = 0;
   len = strlen(s);
   str = (char *)malloc((len+1)* sizeof(char));
   strcpy(str, s);
   for(i=0; i<len; i++){
         for(j=0; j<len; j++){
           if(i==j){

          }
          else if(str[i] == str[j]){
              for(k=j; k<len-1; k++){
                  str[k] = str[k+1];
              }
              cnt++;
              str[len-1] = '\0';
              len--;
          }
      }
  }
  printf("%d\n", len);
  printf("%d\n", cnt);
  printf("%s\n", str);
  len = strlen(str);
  return str;
}
