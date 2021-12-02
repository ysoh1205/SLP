#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_data {
	int thread_id;
	int result;
};

int row_size;
int col_size;

/*** Insert your own matrix & vector data, functions ***/
int** matrix;
int* vector;
int* result;


void *thread_mvm(void *arg) {
	/*** Insert your code ***/
	struct thread_data *t_data = (struct thread_data*)arg;
	int id = t_data->thread_id;
	for(int i=0; i<col_size; i++){
		t_data->result += matrix[id][i]*vector[i];
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Usage: %s <row> <column>\n", argv[0]);
		exit(1);
	}

	row_size = atoi(argv[1]);
	col_size = atoi(argv[2]);
	pthread_t tid[row_size];	
	struct thread_data t_data[row_size];
	int thr_id;
	srand(time(NULL));

	/*** Insert your code ***/
	matrix = malloc(sizeof(int*)*row_size);
	for(int i=0; i<row_size; i++){
		matrix[i] = malloc(sizeof(int)*col_size);
		for(int j=0; j<col_size; j++){
			matrix[i][j]=rand()%10;
		}
	}
	
	vector = malloc(sizeof(int)*col_size);
	for(int i=0; i<col_size; i++){
		vector[i]=rand()%10;
	}

	result = malloc(sizeof(int)*row_size);
	
	printf(" *** Matrix ***\n");
	for(int i=0; i<row_size; i++){
		for(int j=0; j<col_size; j++){
			printf("[ %d ] ", matrix[i][j]);
		}
		printf("\n");
	}
	
	printf(" *** Vector ***\n");
	for(int i=0; i<col_size; i++){
		printf("[ %d ]\n", vector[i]);
	}

	for(int i=0; i<row_size; i++){
		t_data[i].thread_id=i;
		t_data[i].result=0;
	}
	for(int i=0; i<row_size; i++){
		if((thr_id=pthread_create(&tid[i], NULL, thread_mvm, (void*)&t_data[i]))){
			printf("ERROR: pthread creation failed.\n");
			exit(1);
		}
	}
	for(int i=0; i<row_size; i++){
		pthread_join(tid[i], NULL);
	}
	for(int i=0; i<row_size; i++){
		result[i]=t_data[i].result;
	}

	printf("\n *** Result ***\n");
	for(int i=0; i<row_size; i++){
		printf("[ %d ]\n", result[i]);
	}


	pthread_exit(NULL);
}

