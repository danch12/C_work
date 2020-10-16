#include <stdio.h>
#include<stdlib.h>

#define HOURSINDAY 24
#define MINSINHOUR 60
#define MIDNIGHT (24*60)

typedef int time;

time total_mins(time hours,time mins);
void validate(time hours, time mins);

int main(void)
{
  time first_hours,first_mins, second_hours, second_mins;
  time total_first_mins, total_second_mins;
  time diff_mins;
  do
  {
     printf("enter two times\n");
  }while(scanf("%d:%d %d:%d", &first_hours, &first_mins, &second_hours, &second_mins)!=4);

  validate(first_hours,first_mins);
  validate(second_hours,second_mins);

  total_first_mins= total_mins(first_hours,first_mins);
  total_second_mins=total_mins(second_hours,second_mins);



  if (total_first_mins> total_second_mins)
  {
    diff_mins = MIDNIGHT - total_first_mins + total_second_mins;
  }
  else
  {
    diff_mins = total_second_mins-total_first_mins;
  }
  printf("\n");
  printf("the time difference is %d:%d\n", diff_mins/MINSINHOUR, diff_mins%MINSINHOUR);
  return 0;

}

void validate(time hours, time mins)
{
   if(hours<0||hours>HOURSINDAY)
   {
      exit(1);
   }
   if(mins<0||mins>MINSINHOUR)
   {
      exit(1);
   }
}

time total_mins(time hours,time mins)
{
   return (MINSINHOUR*hours)+mins;
}
