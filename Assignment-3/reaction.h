#include <stdio.h>
#include <pthread.h>

struct reaction {
	// FILL ME IN
	pthread_mutex_t H_mutex ;	
	pthread_cond_t H_cond;
	pthread_cond_t O_cond;
	pthread_cond_t H2_cond;
	
};

void reaction_init(struct reaction *reaction);

void reaction_h(struct reaction *reaction);

void reaction_o(struct reaction *reaction);
