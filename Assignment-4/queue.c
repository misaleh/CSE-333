#include "queue.h"
extern process *current_process;
void enqueue(int *first , int *last ,process *element, process *arr[]) 
{
	arr[*last] = element;
	//printf("enQ = %d",arr[*last]->id);
	(*last) ++;
}
int dequeue(int *first , int *last , process *arr[])
{
//	printf("dequeue last = %d , first = %d \n",*last,*first);
//	printf("deQ = %d\n",arr[*first]->id);
	if((*last) == (*first))
	{
		return 0;
	}
	else
	{	
		current_process = arr[*first];
		//printf("deQ element = %d\n",current_process->id);
		//printf("deQ element address = %d\n",current_process);
		(*first) ++ ;
		return 1 ;
	}
}
