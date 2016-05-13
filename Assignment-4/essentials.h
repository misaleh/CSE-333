#ifndef ESSENTSIALS_H
#define ESSENTSIALS_H
#define FCFS 0
#define RR 1

typedef struct process
{
  int id;  // the id of process
  int cpu_time;
  int cpu_time1;
  int io_time;
  int arrival_time;  //change when blocked
  int arrival_original ; //does not change used to calculate turn arround time
  int blocked_state; //if blocked 
  int blocked_time; // time of blocking
  int to_release; //to be released from block next clock
  int to_block; //will block next tick
  int ready_state; //if ready
  int finished;  //if finished
  int turnarround;
  int to_finish ;
  int is_running; //current process is running this tick
  int sched_rr; //flag set when sheduled when its time finished (in round robin)
  int running_time; // the running time of the process from starting till current moment it is reseted when blocked
}process;

void FCFS_schedule(process p[],int number_processes);/*array of processes and thier number as argument*/
void RR_schedule(process p[],int number_processes,int quantum);

#endif
