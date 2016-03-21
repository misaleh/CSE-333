#ifndef ESSENTAIL_H
#define ESSENTAIL_H

#define True 1
#define Ssize  513
#define debug1
#define STRING_MAX 512
#define COMMANDS_MAX 255
#define BACKGROUND 0
#define FOREGROUND 1
#define INVAILD_STATUS  2
#define ERROR_FORKING   3
#define CHILD_PID   	0
#define EXIT_RETURN     -1



char CommandSplit(char[Ssize], char*[COMMANDS_MAX],char *); /* the function that splits the command it has 3 arguments
the input string and the output array of strings and the length of the output array with the NULL at the end,
and return if the task is background(0) or foreground(1)
*/
char CdExecute(char*); /*takes 2 arguments and the status*/
char CommandExecuteNormal(char*[COMMANDS_MAX] , char);/*command and its status it takes  the command to execute and the its status (background or foreground) and returns the status of excution 
it returns
0 if ok
1 if invailed command
2 if the status of input command is neither 0 nor 1
3 if there is problem in forking the child
*/
char CommandExecute(char*[COMMANDS_MAX] , char); /*normal command or cd or exit*/

#endif
