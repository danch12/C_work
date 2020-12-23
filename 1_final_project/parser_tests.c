#include "parser_funcs.h"


#define MAXTESTCAP 100
#define MAXTESTLEN 50
/*used for testing*/
bool concat_word_cont(word_cont* to_concat, char target[MAXLEN]);
word_cont* init_word_cont(void);


int main(void)
{
   word_cont* test_cont;
   FILE* fp;
   int test_lines;
   char test_str[MAXLEN];
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"1234");
   assert(valid_num(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(valid_varnum(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"123.4");
   assert(valid_num(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"123.a");
   assert(!valid_num(test_cont));
   test_cont->position=0;
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"");
   assert(!valid_num(test_cont));
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"1a");
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0],"a1");
   assert(!valid_varnum(test_cont));
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0],"1a1");
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0]," ");
   assert(!valid_num(test_cont));
   assert(test_cont->position==0);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RT");
   strcpy(test_cont->words[1],"1234");
   assert(valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"RT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;


   strcpy(test_cont->words[1],"90RT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"12");
   strcpy(test_cont->words[0],"RG");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"LRG");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"LT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;


   strcpy(test_cont->words[0],"LT");
   assert(valid_mv(test_cont,"LT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"FD");
   assert(valid_mv(test_cont,"FD"));
   test_cont->position=0;

   assert(valid_instruct(test_cont));
   assert(test_cont->position=2);
   test_cont->position=0;

   strcpy(test_cont->words[0],"FG");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);

   strcpy(test_cont->words[1],"121RT");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);

   strcpy(test_cont->words[1],"121RT");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);


   test_cont->position=0;
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"2654");
   strcpy(test_cont->words[2],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==3);


   test_cont->position=0;
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==5);

   test_cont->position=1;
   assert(!valid_instructlist(test_cont));

   test_cont->position=2;
   assert(valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"2");
   assert(!valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==5);

   test_cont->position=0;
   strcpy(test_cont->words[1],"}");
   assert(!valid_instructlist(test_cont));
   strcpy(test_cont->words[1],"2654");

   strcpy(test_cont->words[0],"}");
   assert(valid_instructlist(test_cont));
   strcpy(test_cont->words[0],"FD");
   assert(test_cont->position==1);

   test_cont->position=0;
   assert(!valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"2654");
   strcpy(test_cont->words[3],"}");
   assert(valid_main(test_cont));
   assert(test_cont->position==4);

   test_cont->position=0;
   strcpy(test_cont->words[1],"}");
   assert(valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   strcpy(test_cont->words[1],"FD");
   assert(!valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"+");
   assert(valid_op(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"++");
   assert(!valid_op(test_cont));
   strcpy(test_cont->words[0],"+-");
   assert(!valid_op(test_cont));
   strcpy(test_cont->words[0],"/-");
   assert(!valid_op(test_cont));
   assert(!valid_varnum(test_cont));

   strcpy(test_cont->words[0],"AA");
   assert(!valid_var(test_cont));
   strcpy(test_cont->words[0],"A");
   assert(valid_var(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"Aa");
   assert(!valid_var(test_cont));
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"a");
   assert(!valid_var(test_cont));
   strcpy(test_cont->words[0],"Z");
   assert(valid_var(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(valid_varnum(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"/");
   strcpy(test_cont->words[1],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==2);
   test_cont->position=0;
   /*doesnt have to make sense to be valid*/
   strcpy(test_cont->words[1],"2");
   strcpy(test_cont->words[2],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   test_cont->position=0;

   strcpy(test_cont->words[1],"+");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   test_cont->position=0;

   strcpy(test_cont->words[3],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   assert(valid_polish(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[1],"A");
   assert(valid_polish(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[2],"A");
   assert(valid_polish(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[2],"");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[2],"A");
   test_cont->position=0;
   strcpy(test_cont->words[1],"FD");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],"1A");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],"+1");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],";;");
   assert(!valid_polish(test_cont));
   /*no semi colon*/
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[3],"A");
   assert(!valid_polish(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"/");
   strcpy(test_cont->words[4],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==5);
   test_cont->position=0;
   assert(valid_instruct(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],";");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==4);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==4);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"a");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"set");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"aa");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"=:");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[2],"A");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[2],"");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   free_word_cont(test_cont);

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==8);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"40");
   strcpy(test_cont->words[9],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==10);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"3");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"9");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"40");
   strcpy(test_cont->words[16],"}");
   strcpy(test_cont->words[17],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[3],"10");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;

   strcpy(test_cont->words[14],"LT");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[17],"");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[17],"}");


   strcpy(test_cont->words[16],"");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[16],"}");

   strcpy(test_cont->words[13],"}");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[13],"{");

   strcpy(test_cont->words[8],"2");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[8],"A");

   strcpy(test_cont->words[7],"OD");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[7],"DO");
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"SET");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],":=");
   strcpy(test_cont->words[6],"30");
   strcpy(test_cont->words[7],"/");
   strcpy(test_cont->words[8],";");

   strcpy(test_cont->words[9],"DO");
   strcpy(test_cont->words[10],"B");
   strcpy(test_cont->words[11],"FROM");
   strcpy(test_cont->words[12],"A");
   strcpy(test_cont->words[13],"TO");
   strcpy(test_cont->words[14],"30");
   strcpy(test_cont->words[15],"{");

   strcpy(test_cont->words[16],"}");
   strcpy(test_cont->words[17],"}");

   assert(valid_main(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;

   strcpy(test_cont->words[17],"DO");
   strcpy(test_cont->words[18],"C");
   strcpy(test_cont->words[19],"FROM");
   strcpy(test_cont->words[20],"A");
   strcpy(test_cont->words[21],"TO");
   strcpy(test_cont->words[22],"30");
   strcpy(test_cont->words[23],"{");
   strcpy(test_cont->words[24],"LT");
   strcpy(test_cont->words[25],"30");
   strcpy(test_cont->words[26],"FD");
   strcpy(test_cont->words[27],"30");
   strcpy(test_cont->words[28],"}");
   strcpy(test_cont->words[29],"}");
   strcpy(test_cont->words[30],"}");
   assert(valid_main(test_cont));
   assert(test_cont->position==30);
   test_cont->position=0;

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"40");


   strcpy(test_cont->words[3],"DO");
   strcpy(test_cont->words[4],"B");
   strcpy(test_cont->words[5],"FROM");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"TO");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"SET");
   strcpy(test_cont->words[12],"A");
   strcpy(test_cont->words[13],":=");
   strcpy(test_cont->words[14],"30");
   strcpy(test_cont->words[15],"/");
   strcpy(test_cont->words[16],";");
   strcpy(test_cont->words[17],"}");
   assert(valid_main(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;

   strcpy(test_cont->words[1],"{");
   assert(!valid_main(test_cont));
   strcpy(test_cont->words[1],"FD");
   test_cont->position=0;

   free_word_cont(test_cont);


   test_lines=0;
   fp=get_file_words("test_files/test_sizes/7words.txt",&test_lines);
   fclose(fp);
   assert(test_lines==7);

   fp=get_file_words("test_files/test_sizes/5_words.txt",&test_lines);
   fclose(fp);
   assert(test_lines==6);

   fp=get_file_words("test_files/test_sizes/10words.txt",&test_lines);
   fclose(fp);
   assert(test_lines==10);

   test_cont=read_in_file("test_files/test_sizes/7words.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"1234567")==0);
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_sizes/5_words.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"Word1word2;word4.Word6")==0);
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_sizes/10words.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"W2{}5678jnjffnvndkdke10")==0);
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/using_variables.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{DOAFROM1TO100{SETC:=A1.5*;FDCRT62}}")==0);
   assert(valid_main(test_cont));
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/parser/valid/adding_loops.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{DOAFROM1TO8{FD30LT45}}")==0);

   assert(valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/nested_loops.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{DOAFROM1TO50{FDART30DOBFROM1TO8{SETC:=A5/;FDCRT45}}}")==0);
   assert(valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/more_do.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{DOAFROM1TOC{FD50}}")==0);
   assert(valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{SETA:=++34;}")==0);
   assert(valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish_2.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{SETA:=B+33+;}")==0);
   assert(valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/invalid/bad_spacing.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"{DOAFROM1TO50{FD40}}")==0);
   assert(!valid_main(test_cont));
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/invalid/missing_bracket.txt");
   concat_word_cont(test_cont,test_str);
   assert(strcmp(test_str,"FD50LT30}")==0);
   assert(!valid_main(test_cont));
   free_word_cont(test_cont);
   return 0;
}




/*pretty much just for testing so we dont
deal with files until end*/
word_cont* init_word_cont(void)
{
   word_cont* n_cont;
   int i;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   n_cont->capacity=MAXTESTCAP;
   n_cont->words= (char**)safe_calloc(MAXTESTCAP,sizeof(char*));
   for(i=0;i<MAXTESTCAP;i++)
   {
      n_cont->words[i]=(char*)safe_calloc(MAXTESTLEN,sizeof(char));
      n_cont->words[i][0]='\0';
   }
   n_cont->position=0;
   return n_cont;
}

/*used for testing*/
bool concat_word_cont(word_cont* to_concat, char target[MAXLEN])
{
   int i;
   if(to_concat)
   {
      target[0]='\0';
      for(i=0;i<to_concat->capacity;i++)
      {
         strcat(target,to_concat->words[i]);
      }
      return true;
   }

   return false;
}
