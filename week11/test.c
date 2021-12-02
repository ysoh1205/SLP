#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4

void *thread(void *arg){
	long id = (long)arg;
	printf("thread#%ld: Hello Thread!\n", id);
	pthread_exit(NULL);
}

int main(){
	pthread_t tid[NUM_THREADS];
	long t;

	for(t=0; t<NUM_THREADS; t++){
		printf("main: creating thread#%ld\n", t);
		if(pthread_create(&tid[t], NULL, thread, (void*)t)){
			printf("ERROR: pthread creation failed.\n");
			exit(1);
		}
	}

	for(t=0; t<NUM_THREADS; t++){
		pthread_join(tid[t], NULL);
	}
	printf("main: bye bye!\n");
	pthread_exit(NULL);
}
