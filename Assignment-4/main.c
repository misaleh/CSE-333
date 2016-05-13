#include "essentials.h"
#include "queue.h"
#include "files.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG1

int i,j,k;
process p[15];  /*array of process in file*/
int sched_type = 0;   /*FCFS or RR*/
int quantum = 0;    /*quantum time in case of RR*/
int number_processes = 0; /*number of processes in the files*/
process *current_process;
char *OUTFILE ;
int main(int argc ,char *argv[])
{
  char* filename; /*the name of the file entered in command line*/ 
  filename = (char*) malloc(12*sizeof(char));
  OUTFILE  = (char*) malloc(20*sizeof(char));

  sched_type = atoi(argv[1]); /*the scheduling is the first argument*/ 
  if(sched_type == FCFS) /*if users choosed FCFS (0)*/
    {
	filename = argv[2];/*the name of the file is the second argument */
    }
  else if(sched_type == RR) /*if users chooses RR (1)*/
   {
	quantum =atoi(argv[2]); /*the quantum is the second argument*/
	filename = argv[3]; /* the name of the file is the third argument*/
   }
  else
   {
	perror("Wrong scheduling type"); /*print error if differnet number (other than 0 or 1)is choosen*/
   }

readFile(filename,&number_processes,p); /*will read each element and put the parmeters of process in a struct making array array of structs*/
/*each process will run for simulation time ,block for i/o time then run for simulation time again */

if(sched_type == FCFS)
{
	 OUTFILE =  strcat(filename,"FCFS.out"); 
	 FCFS_schedule(p,number_processes);
}
else if(sched_type == RR)
{
  	OUTFILE = strcat(filename,"RR.out"); 
	RR_schedule(p,number_processes,quantum);
}
return 0;
}
