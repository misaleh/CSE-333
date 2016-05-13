#include "files.h"
#include <stdio.h>
void readFile(char *fileName ,int *number_processes,process p[]){  
    int i = 0;
    int process_counter = 0;
    int c = 10;
    FILE *in= fopen(fileName, "r") ; 

  do /*read each char from file and assings it to the right variable in the array of the struct process*/
{ 
	fscanf(in , "%d", &c) ;
	//printf("c = %d\n",c);
	if(i == 0)
	{
		p[process_counter].id = c;
		i++;
	}
	else if( i == 1)
	{
		p[process_counter].cpu_time =c;	
		p[process_counter].cpu_time1 =c;

		i++;
	}
	else if(i == 2)
	{
		p[process_counter].io_time = c;
		if(p[process_counter].io_time == 0) //new mostafa
		{
		 p[process_counter].cpu_time1 = 0;
		 p[process_counter].cpu_time = 2*p[process_counter].cpu_time;
		}
		i++;
	}
	else if(i == 3)
	{
		p[process_counter].arrival_time =c;
		p[process_counter].arrival_original = c;
		i = 0;
		++process_counter; 
	}
} while(getc(in) != EOF);
 	*number_processes = process_counter; 

    fclose(in);
}


/*
void writeToFile(int rowsNum, int colsNum, char *fileName){
	FILE *out=fopen(fileName, "w") ;
	   int i,j=0;
	    for (i=0;i<rowsNum;i++){
		for (j=0;j<colsNum;j++){
		    fprintf (out ,"%d\t" ,cMatrix[i][j]);
		}
		fprintf(out ,"\n");
	    }
	    fclose (out);
}*/
