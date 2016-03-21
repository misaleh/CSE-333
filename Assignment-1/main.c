#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include "Essential.h"

char input_stream[Ssize];  /*the input command from shell*/
char *output_stream[COMMANDS_MAX];
char Process_Type ; /* foreground --> 1, background --> 0*/
char j =0; /* index used for for loop*/
char command_size = 0;
char Execute_return = 0;
FILE *OutCommnadF; /*pointer to File used to the read the output of excuted commnad*/
char* USER; /* the name of the user*/
char* HomePath; /*the path of the home as is /home/USER */
int main()
{	
  HomePath = (char*) malloc(12*sizeof(char));
  USER = (char*) malloc(12*sizeof(char));
  printf("\n");
  HomePath =  strcpy(HomePath,"/home/"); 
  OutCommnadF = popen("whoami","r");
  fgets(USER,20,OutCommnadF);
  pclose(OutCommnadF);
  USER = strtok(USER, "\n");
  strcat(HomePath ,USER);


  while (True)
  {
    printf("%s>",USER);                /*the default of  shell*/
    fgets(input_stream, Ssize, stdin); /*get the input from user*/
    if (strlen(input_stream) > STRING_MAX) fputs("Too long command\n", stderr); /*print error if command is too long*/
    else if (strlen(input_stream) == 1) continue; /*if no command is entered go back to the loop*/
     input_stream[strcspn(input_stream, "\n")] = 0; //remove enter from the end of the string

     Process_Type =  CommandSplit(input_stream,output_stream,&command_size);
#ifdef debug
   	printf("\nthe enterd command was: %s\n", input_stream); /*check the input for debugging purpose*/
        printf("The output array is:\n");
        for(j = 0; j < (command_size) ; j++)
        {
        printf("output_stream[%d] = %s\n",j,output_stream[j]);
        }
        printf("the process type is %d\n",Process_Type);
#endif
	 Execute_return  = CommandExecute(output_stream,Process_Type);
	if(Execute_return == -1)
	{
		break ; 
	}



  }
  return 0;
}



