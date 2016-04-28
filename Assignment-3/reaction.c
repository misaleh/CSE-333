#include <pthread.h>
#include "reaction.h"
#include <stdio.h>
#define Water_condition (H_count >= 2) && (O_count >= 1)

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
int H_count =0 ;
int O_count = 0;
void make_water();


void reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	/*intit mutex and condition variable*/
	struct reaction *temp;
	temp = reaction;
	pthread_mutex_init(&(temp->H_mutex),NULL);
	pthread_cond_init(&(temp->H_cond),NULL);
	pthread_cond_init(&(temp->H2_cond),NULL);
	pthread_cond_init(&(temp->O_cond),NULL);
}


void reaction_h(struct reaction *reaction)
{	
	
	// FILL ME IN
	struct reaction *temp;
	temp = reaction;
	
	pthread_mutex_lock(&(temp->H_mutex));
	H_count ++;
	if(Water_condition)
	{
		while(Water_condition)
	{
		H_count -= 2;
		O_count --;
		make_water();
		pthread_cond_signal(&(temp->H_cond)); 
		pthread_cond_signal(&(temp->O_cond));
	}
	}
	else
	{
		pthread_cond_wait(&(temp->H_cond),&(temp->H_mutex));
	}
	pthread_mutex_unlock(&(temp->H_mutex));
	

	return;
	
}


void reaction_o(struct reaction *reaction)
{
	// FILL ME IN
	
	struct reaction *temp;
	temp = reaction;	
	pthread_mutex_lock(&(temp->H_mutex));
	O_count++;
	if(Water_condition)
	{
		while(Water_condition)
	{
		H_count -= 2;
		O_count --;
		make_water();
		pthread_cond_signal(&(temp->H_cond)); 
		pthread_cond_signal(&(temp->H_cond));
	}
	}
	else
	{
		pthread_cond_wait(&(temp->O_cond),&(temp->H_mutex));
	}
	pthread_mutex_unlock(&(temp->H_mutex));

	return;
}


