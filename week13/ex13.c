#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int point_in_circle = 0;
int radius = 1;
void *inCircle(void* arg){
    double x, y;
    pthread_mutex_lock(&m);
    x = rand()/(double)RAND_MAX;
    y = rand()/(double)RAND_MAX;
    if(x*x + y*y <= radius) point_in_circle++;
    pthread_mutex_unlock(&m);
}

int main(int argc, char* argv[]){
    int thread_num = atoi(argv[1]);
    int point_num = atoi(argv[2]);

    srand(time(NULL));
    pthread_t tid[thread_num];
    for(int i=0; i<point_num; i++){
        for(int j=0; j<thread_num; j++){
            pthread_create(&tid[j], NULL, &inCircle, NULL);
        }
        for(int j=0; j<thread_num; j++){
            pthread_join(tid[j], NULL);
        }
    }
    double pi = 4*point_in_circle / (double)(point_num*thread_num);
    printf("%f\n", pi);
}