
**OS** : Ubuntu 14.04


**Toolchain** : GNU Toolchain 


**Binary File** : Shell



The Source is associated with a makefile to compile the source code and generate binary file called "Shell".
The code is a UNIX shell, that interfaces with the user to run UNIX commands.

#### **Requirements:** 

 - The Shell should terminate when the user enter `exit` command.
 -  The Shell should handle errors without termination.
 - These cases are classified as errors and the user should be informed about it by printing in **stderr**:
    * Command linelonger than 512 charater.
    * Command does not exist.
 - These cases are not errors but should be handled:
    * Empty command line.
    * Multiple white spaces in a command line.
    * White space before and after `&`.
    * If `&` appeared in the middle of a command line, it should be treated normally.

----------
#### **Code Specifications** :
The code is associated with debugging mode that is not enabled by default, to enable it add  ```  #define debug```  in *Essential.h*  file.
	The debugging is acts as the normal mode in addition to printing new information about variables indicating  the flow of the code	.
	The debugging mode can be used to extend  the shell in the future.

The shell starts by printing the current user names as ```   UserName ```  .
It waits for the user to enter command line to be executed and waits for the user again.
The shell is capable of handling normal commands including ```   cd ```  .
It cannot handle ```   >> ```   , ```  | ```   , ```  > ```   etc.
To exit the shell type ```   exit```    in the shell, it will ignore any other parameters after it.
If any entered command line is longer 512 charater it will print an error.
The shell can handle assigning tasks to run in the background.

#### **Implementation** :

The code consists of 3 files : *main.c* , *Essential.h* and *Essential.c*.
##### ***main.c***
The main code start with getting the current user name which is used to display the shell as ```  UserName```   and when changing the directory to home
```
HomePath =  strcpy(HomePath,"/home/"); 
  OutCommnadF = popen("whoami","r");
  fgets(USER,20,OutCommnadF);
  pclose(OutCommnadF);
  USER = strtok(USER, "\n");
  strcat(HomePath ,USER);
  ```
First the code initialize the   ```   HomePath ```    variable with 
```/home/``` which is the default for Unix systems.
Then it open file running ``` whoami``` command in it which returns the name of the current user, then, it reads it  from the file and remove the ``` '\n'```	if it exists, and  saves it in ``` USER``` variable, finally the  ``` HomePath``` is concatenated with ``` USER ``` producing the home directory of current user.

Afterward the code Enters infinite loop ```while (True)``` of the sell, it prints the name of the user then ``` > ``` and wait for the user to enter a command line.

```
printf("%s>",USER);    
fgets(input_stream, Ssize, stdin);  
  ```  
  
After the user enters the command it performs some checks on it, if the input is too long it prints error in ``` stderr``` , if the command is empty it jumps back to the start of the loop.


  ```
  if (strlen(input_stream) > STRING_MAX) fputs("Too long command\n", stderr); 
  else if (strlen(input_stream) == 1) continue;
  ```
Then it removes the ```'\n'``` from the end of the string as it is never used.
A function call to ```CommandSplit``` is done , the function splits the  ```input_stream``` with delimiter space adding null to its end, removing the background operator```&  ```  if found.
it returns the output to ```output_stream``` it also return the type of the command line (background or foreground) and the number of strings in the command line.

```
input_stream[strcspn(input_stream, "\n")] = 0; 
Process_Type = CommandSplit(input_stream,output_stream,&command_size);
```
Then a function call to is done which takes the output of the last command as an argument to execute and the type of the process, it returns 1 if the user pressed ```exit``` .
If the user pressed ```exit``` it will break the infinite loop and the shell will terminate.

```
Execute_return  = CommandExecute(output_stream,Process_Type);
	if(Execute_return == -1)
	{
		break ; 
	}
```
##### ***Essential.h***
It is a header file that contains essential definitions  as ```STRING_MAX ```  , ```CHILD_PID ```
It is optional to add to  ```#define debug``` to print more statements about the execution. 
It also contain function declarations:
>char CommandSplit(char[Ssize], char*[COMMANDS_MAX],char *); 
>char CdExecute(char*); 
>char CommandExecuteNormal(char*[COMMANDS_MAX] , char);
>char CommandExecute(char*[COMMANDS_MAX] , char);

##### ***Essential.c***
This file contain the implementation of the functions declared in *Essential.h*.
First in order to use ```HomePath``` in this file it is declared as ```extern``` .

```
extern char*  HomePath; 
```

The first function :

> __char CommandSplit(char InputCommand[Ssize], char *OutputCommand[COMMANDS_MAX], char * command_size)__


Arguments:

 - ```InputCommand[Ssize] ```: The input string of the command line.
 - ```OutputCommand[COMMANDS_MAX]```: Array of strings after spiting the input and editing it.
 - ```command_size```: Pointer that will hold the number of strings of the output array.

First, the input command line is splitted with delimeter  space adding  ```NULL``` at the end.
```
while(OutputCommand[i] != NULL) 
    {
        i++;
        OutputCommand[i] = strtok(NULL, " "); 
    }
  ```
 ``` command_size```: will hold the last index which indicates the length of the array.
 ``` last_command```: will hold the last string in the array .
  ```last_string_len```: will hold the length  the of the last of string.
  ```last_char```: will hold the last char in the last string.
   ```
   *command_size = i; 
    last_command =  OutputCommand[i - 1];
    last_string_len = strlen(last_command); 
    last_char = last_command[last_string_len-1];
  ```
 To check if the command lined is  a background there is three cases:
 

 1. The user enter  a command line, then space then  ```&```as ```command line &```
 2. The user enter a command line then ```&``` without space as  ```command line&```


The last command is compared with ```&```  , if they are equal so this command is in the background , or if the last char of the last command is  ```&```, it will be also in the background.

```
if((!strcmp(last_command,background_char))) 
    {
    command_state=0;
    OutputCommand[(*command_size)-1] = '\0';
    } 
    else if(last_char == '&')
     {
            command_state = 0;
	    last_command[last_string_len-1] = '\0'; 
     }
    else  {command_state = 1; }
    return command_state;
```
The second function:
>__char CommandExecuteNormal(char *Command[COMMANDS_MAX],char Process_Type)__

Arguments:

 - ```Command[COMMANDS_MAX]```: Array of strings of the command line and its output.
 - ```char Process_Type```:The type of the process; background or foreground.

This function executes normal commands which ares supported by ```execvp()``` function.
First the function forks new process to execute the command and let the main process to handle the shell.
```pid = fork();```

Then in the child process, it will execute the command line and returns then it terminates its self.
 ```
 if(pid == CHILD_PID)
	{
     	   error_cmd = execvp(Command[0], Command);
	   if(error_cmd)
	{
		fputs("Inavild Command\n", stderr);
		exit(EXIT_FAILURE); 
	}
	  exit(EXIT_SUCCESS); 
	}
```
For the parent process, it will wait for the child if the command line was foreground, and will continue if it was background.
  ``` 
  else if(pid > 0) 
	{
		if(Process_Type == BACKGROUND) 
		{
		}
		else if(Process_Type == FOREGROUND)
		{
		waitpid(pid, &status, WUNTRACED);
		}
		else
		{
		fputs("Error in state\n", stderr);
		Return_Status =  INVAILD_STATUS;
		}	
	}
```
If there is error in forking, it will print in the ```stderr```
```  
else 
	{
	fputs("Error in fork\n", stderr);
	Return_Status =  ERROR_FORKING ;
	}
```
Third function:
>__char CdExecute(char* Path)__

Arguments:
 - ```Path```: The path of the directory, that will be changed to.

If there is no path; the user pressed ```cd```only, it will change the path to the home directory, if not it will change the directory to the new path, it will print in the ```stderr``` if the path is wrong.
```
if(Path == 0)
{
	ex_state = chdir(HomePath); 
}
else
{
	ex_state = chdir(Path); 
}
if(ex_state != 0)  
	{
		fputs("Error in changing directory\n", stderr);
	}
return Return_Status; 
```
The Fourth function: 
> __char CommandExecute(char *Command[COMMANDS_MAX] , char Process_Type)__

Arguments:

 - ```Command[COMMANDS_MAX]```: Array of strings of the command line and its output.
 - ```char Process_Type```:The type of the process; background or foreground.

This is the function that is called in *main.c* it will choose if the function to be called based on the command type.
If the command is ```cd``` it will call ```CdExecute```.
If it was exit it will `return -1` to that will break the loop in *main.c*.
Otherwise it will call ```CommandExecuteNormal``` that executes the other commands.
```
	if(!(strcmp(Command[0],"cd")))
	{
		Return_Status = CdExecute(Command[1]);
	}
	else if (!(strcmp(Command[0],"exit")))
	{
		Return_Status = -1; 
	}
	else
	{
		CommandExecuteNormal(Command,Process_Type); 
	}
	return Return_Status;
```







