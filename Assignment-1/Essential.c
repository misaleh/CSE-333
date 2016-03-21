#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include "Essential.h"
/* ******************************************* CommandSplit ****************************************************** */
/* *************************************************************************************************************** */
extern char*  HomePath;
char CommandSplit(char InputCommand[Ssize], char *OutputCommand[COMMANDS_MAX] ,char * command_size)
{
    char i = 0;
    char* last_command ;  /*to check background or foreground*/
    char command_state = 0 ; /*foreground(1) or background(0)*/
    char  last_char = 0;
    unsigned char  last_string_len;
    char  background_char[] = {'&','\0'} ; /*if the user pressed '&' then space then enter*/
    char  background_char1[] = {'&','\n','\0'} ; /*if the user pressed '&' then enter*/
    char  enter_string[] = {'\n','\0'};
    OutputCommand[0] =   strtok(InputCommand, " ");
    while(OutputCommand[i] != NULL) /* splitting the input*/
    {
        i++;
        OutputCommand[i] = strtok(NULL, " "); /*returns NULL when there is no more split*/
    }
    *command_size = i; /* return the number of strings in the command including NULL*/
    last_command =  OutputCommand[i - 1]; /*the last argument in the command , used to check if foreground or background*/
    last_string_len = strlen(last_command); /*length of the last string in the command */
    last_char = last_command[last_string_len-1]; /*last char in the command*/
    #ifdef debug
    printf("the last word is %s\n",last_command);
    printf("the last char in last command is %c\n",last_char);
    #endif
    /*if there is no & in the first 2 case in the documentaion*/
    if((!strcmp(last_command,background_char))) /*check of enterd &\n or & as indvidual string*/
    {
    command_state=0;
    OutputCommand[(*command_size)-1] = '\0';/* remove the & */
    } 
    else if(last_char == '&')
     {
            command_state = 0;
	    last_command[last_string_len-1] = '\0'; /* remove the & */
     }
    else  {command_state = 1; }
    return command_state;
}

/* ******************************************* CommandExecuteNormal ****************************************************** */
/* *********************************************************************************************************************** */

char CommandExecuteNormal(char *Command[COMMANDS_MAX],char Process_Type)
{
     pid_t pid; /*the return process pid of the child task */
     int status = 0 ;	
     char error_cmd = 0;
     char Return_Status = 0; 
     pid = fork();
     if(pid == CHILD_PID) /*child process*/
	{
     	   error_cmd = execvp(Command[0], Command);
	   if(error_cmd)
	{
		fputs("Inavild Command\n", stderr);
		exit(EXIT_FAILURE); /*exit tge child*/ 
	}
	  exit(EXIT_SUCCESS); /*exit the child*/
	}
    else if(pid > 0) 
	{
		if(Process_Type == 0) /*if background*/
		{
		}
		else if(Process_Type == 1) /*if foreground*/
		{
		waitpid(pid, &status, WUNTRACED);
		}
		else
		{
		fputs("Error in state\n", stderr);
		Return_Status =  INVAILD_STATUS;
		}	
	 
	} 

     else 
	{
	fputs("Error in fork\n", stderr);
	Return_Status =  ERROR_FORKING ;
	}	
	return	Return_Status;
}

/* ******************************************* CdExecute ******************************************************** */
/* ************************************************************************************************************** */

char CdExecute(char* Path)
{


char ex_state = 0;	/*we hold the return of chdir function*/
char Return_Status = 0; /* the return of the function*/	
if(Path == 0)
{
	//printf("your home path is %s\n",HomePath);
	ex_state = chdir(HomePath); 
}
else
{
	ex_state = chdir(Path);  /*change directory*/
}
if(ex_state != 0)  /*if there is an error (will return -1)*/
	{
		fputs("Error in changing directory\n", stderr); /*print to standerd error*/
	}
		
return Return_Status; 
}

/* ******************************************* CommandExecute ****************************************************** */
/* ***************************************************************************************************************** */

char CommandExecute(char *Command[COMMANDS_MAX] , char Process_Type)
{
	char Return_Status = 0; 
	if(!(strcmp(Command[0],"cd")))
	{
		Return_Status = CdExecute(Command[1]); /* return  0 if ok 3 if error forking*/
		
	}
	else if (!(strcmp(Command[0],"exit")))
	{
		Return_Status = -1;   /*retrun -1 if user enterd exit*/
	}
	else
	{
		CommandExecuteNormal(Command,Process_Type);  /*return 0 if ok  2 if invaild status  3 if error forking*/
	}
	return Return_Status;
}
