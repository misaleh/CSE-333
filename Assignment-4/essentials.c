#include "essentials.h"
#include <stdio.h>
#include "queue.h"
/*id  cpu io arrival*/
extern process *current_process;
extern char* OUTFILE ; 

void FCFS_schedule(process p[],int number_processes)
{
	FILE *fp = fopen(OUTFILE,"ab+"); //open the output to print output in it
	int i = 0;
	int tick = 0; /*number of clock cycles*/
	process *ready[50]; //ready queue with size of number of processes
	int first = 0;
	int last = 0;
	int running_status = 0; /*0 if no process running , 1 if there is a process running*/
	int counter_finished = 0;
	float not_busy =0; //cylces where the cpu is not busy ,float for divison
	int ready_count = 0; //used to calculate utlization
	int running_count = 0; //used to calculate utlization

while(counter_finished != number_processes) //while not all the processes have finsished
{	fprintf(fp,"%d",tick);
	 running_count  = 0 ;//nothing running till now
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids) workin without checking queue
	{
		if((p[i].arrival_time) == tick)
		{
			enqueue(&first,&last,&p[i],ready);
			p[i].ready_state = 1; //when a process arrives it is ready to run
			//printf("check arrived at tick %d  process %d \n",tick,p[i].id);
		}
	}
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids)
	{
		if((p[i].blocked_state) == 1) // if there is a blocked process
		{
		//	fprintf(fp,"%d:blocked ",p[i].id);
			p[i].blocked_time  ++; //when a process arrives it is ready to run	
			if(p[i].blocked_time == p[i].io_time)//if it is finished blocking
			    {
				//p[i].blocked_state = 0; 
				p[i].to_release= 1;
				p[i].arrival_time = tick +1 ; //make it as it is newly arrived the next tick
				p[i].cpu_time = p[i].cpu_time1;
				p[i].cpu_time1 = 0;
			    }

		}
	}
		//printf("ba3d blocked \n");
	if(running_status == 0) // no process is running
	{	
		
		if(dequeue(&first,&last,ready)) /*if there is elements in the queue*/
		{
			running_status = 1;
		//	printf("&ready[0] = %d\n",&ready[0]);
		//	printf("current process address = %d\n",current_process);
			
		}
		else
		{
	
		}
	}
	if(running_status == 1)
	{	 running_count =1;
		current_process->is_running =1;// is running now 
		//fprintf(fp,"%d:running ",current_process->id);
		current_process->cpu_time--;	
		//printf("remaining time of process %d is %d\n",current_process->id,current_process->cpu_time);
		current_process->ready_state = 0; // it is running not ready any more
		if(current_process->cpu_time == 0 )
		{
			running_status =0; //no process is running now this process finsished
			if(current_process->cpu_time1 ==0)
			{
				current_process->to_finish = 1; //will finish next tick
				counter_finished ++;
				current_process->turnarround = tick - current_process->arrival_original + 1;
				//printf("Number of finished processes = %d\n",counter_finished);
			}
			else
			{
				//current_process->blocked_state = 1;// the process is blocked now
				current_process->to_block = 1;
				current_process->blocked_time = 0; //intit time to count
			}		
		}

	}	


	ready_count = 0; // to count ready if none and no running then wasted cycle
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids)
	{
		if((p[i].ready_state) == 1)
		{
			fprintf(fp," %d:ready",p[i].id);
			ready_count ++ ;
		}
		else if((p[i].is_running) == 1)
		{
			fprintf(fp," %d:running",p[i].id);
		}
		else if((p[i].blocked_state) == 1)
		{
			fprintf(fp," %d:blocked",p[i].id);
		}

		if((p[i].to_block) == 1)
		{
			p[i].blocked_state =1;
			p[i].to_block = 0 ;
			p[i].is_running = 0 ;

		}
		if((p[i].to_release) == 1)
		{
			p[i].blocked_state =0;

		}
		if((p[i].to_finish) == 1)
		{
			p[i].is_running = 0 ;

		}
		
	}
 	if((ready_count == 0)&&(running_count == 0))
{
	not_busy++;
}
	tick ++ ; //increase the time
	fprintf(fp,"\n");
}
//printf("Number of finished processes = %d\n",counter_finished);
fprintf(fp,"\n");
fprintf(fp,"Finishing time: %d\n",tick-1);
fprintf(fp,"CPU utilization: %.2f",(tick -not_busy)/(float)tick);
for(i = 0 ; i < number_processes; i++)
{
	fprintf(fp,"\nTurnaround process %d: %d",i,p[i].turnarround);
}
fclose(fp);
}




void RR_schedule(process p[],int number_processes,int quantum)
{
	FILE *fp = fopen(OUTFILE,"ab+");
	int i = 0;
	int tick = 0; /*number of clock cycles*/
	process *ready[50]; //ready queue with size of number of processes
	int first = 0;
	int last = 0;
	int running_status = 0; /*0 if no process running , 1 if there is a process running*/
	int counter_finished = 0;
	float not_busy =0; //cylces where the cpu is not busy ,float for divison
	int ready_count = 0; //used to calculate utlization
	int running_count = 0; //used to calculate utlization

while(counter_finished != number_processes) //while not all the processes have finsished
{	fprintf(fp,"%d",tick);
	//printf("%d ",tick);
	 running_count  = 0 ;//nothing running till now
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids) workin without checking queue
	{
		if((p[i].arrival_time) == tick)
		{
			enqueue(&first,&last,&p[i],ready);
			p[i].ready_state = 1; //when a process arrives it is ready to run
			//printf("check arrived at tick %d  process %d \n",tick,p[i].id);
		}
	}
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids)
	{
		if((p[i].blocked_state) == 1) // if there is a blocked process
		{
			//fprintf(fp,"%d:blocked ",p[i].id);
			//printf("%d:blocked ",p[i].id);
			p[i].blocked_time  ++; //when a process arrives it is ready to run	
			if(p[i].blocked_time == p[i].io_time)//if it is finished blocking
			    {
				//p[i].blocked_state = 0; 
				p[i].to_release = 1;
				p[i].arrival_time = tick +1 ; //make it as it is newly arrived the next tick
				p[i].cpu_time = p[i].cpu_time1;
				p[i].cpu_time1 = 0;
			    }

		}
	}
		//printf("ba3d blocked \n");
	if(running_status == 0) // no process is running
	{	
		
		if(dequeue(&first,&last,ready)) /*if there is elements in the queue*/
		{
			running_status = 1;
		//	printf("&ready[0] = %d\n",&ready[0]);
		//	printf("current process address = %d\n",current_process);
			
		}
		else
		{
	
		}
	}
	if(running_status == 1)
	{	 running_count =1;
		current_process->is_running =1;// is running now 
		//printf("%d:running ",current_process->id);
		current_process->cpu_time--;	//decrement the original cpu time it should run on the cpu
		current_process->running_time++ ; //the time process is running on the processor
		//printf("remaining time of process %d is %d\n",current_process->id,current_process->cpu_time);
		current_process->ready_state = 0; // it is running not ready any more

		if(current_process->cpu_time == 0 )
		{
			running_status =0; //no process is running now this process finsished
			if(current_process->cpu_time1 ==0)
			{
				current_process->to_finish = 1;
				counter_finished ++;
				current_process->turnarround = tick - current_process->arrival_original + 1;
				//printf("Number of finished processes = %d\n",counter_finished);
			}
			else
			{
				//current_process->blocked_state = 1;// the process is blocked now
				current_process->to_block = 1; // will block next tick
				current_process->blocked_time = 0; //intit time to count
			}		
		}
		else if(current_process->running_time == quantum) // if its time is finished on the cpu
			{
				running_status =0; //no process is running now this process finsished
				current_process->running_time = 0; // reset its timer
				current_process->sched_rr = 1 ; // set the flag to 1
 			}
	}	


	ready_count = 0; // to count ready if none and no running then wasted cycle
	for( i = 0 ; i < number_processes; i++) // is new process arrived push in the queue (arranged by ids)
	{
		if((p[i].ready_state) == 1)
		{
			fprintf(fp," %d:ready",p[i].id);

			ready_count ++ ;
		}
		else if((p[i].is_running) == 1)
		{
			fprintf(fp," %d:running",p[i].id);
		}
		else if((p[i].blocked_state) == 1)
		{
			fprintf(fp," %d:blocked",p[i].id);

		}
		if(p[i].sched_rr == 1)
		{
			p[i].ready_state = 1; //the process removed by sceduler is back to ready
			p[i].arrival_time = tick+1; //it will arrive the next clock
			p[i].sched_rr = 0 ;
			p[i].is_running = 0 ;			
			
		}
		if((p[i].to_block) == 1)
		{
			p[i].blocked_state =1;
			p[i].to_block = 0 ;
			p[i].is_running = 0 ;

		}
		if((p[i].to_release) == 1)
		{
			p[i].blocked_state =0;

		}
		if((p[i].to_finish) == 1)
		{
			p[i].is_running = 0 ;

		}
		
	}
 	if((ready_count == 0)&&(running_count == 0))
{
	not_busy++;
}
	tick ++ ; //increase the time
	fprintf(fp,"\n");
	//printf("\n");
}
//printf("Number of finished processes = %d\n",counter_finished);
fprintf(fp,"\n");
fprintf(fp,"Finishing time: %d\n",tick-1);
fprintf(fp,"CPU utilization: %.2f",(tick -not_busy)/(float)tick);

//printf("Finishing time: %d\n",tick-1);
//printf("CPU utilization: %.2f\n",(tick -not_busy)/(float)tick);
for(i = 0 ; i < number_processes; i++)
{
	fprintf(fp,"\nTurnaround process %d: %d",i,p[i].turnarround);
	//printf("Turnaround process %d: %d\n",i,p[i].turnarround);
}

}



