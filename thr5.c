#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_barrier_t barrier;

typedef struct{
   int index;
   int n;
   int* array;
}THR_PARAM;

void* threadFun(void* arg){

    printf("Thread %lu waiting at the barrier...\n", pthread_self());
    pthread_barrier_wait(&barrier);
    printf("Thread %lu passed the barrier...\n", pthread_self()); 

    THR_PARAM param = *(THR_PARAM*)arg;
    int i, sum = 0;
    int x = rand() % 100;

    pthread_mutex_lock(&mutex);

    param.array[param.index] = x;
    param.array[param.n + param.index] = x;

    for(i = 0; i < 2 * param.n; i++){
    	sum += i;
    }

    printf("Thread %lu calculated the sum %d\n", pthread_self(), sum);

    pthread_mutex_unlock(&mutex);

    for(i = 0; i < 2 * param.n; i++){
    	printf("%d ", param.array[i]);
    }
    printf("\n");
    printf("^ This was thread %lu\n", pthread_self());

    free(arg); 
    return 0;
}

int main(int argc, char** argv){
    
    if(argc != 2){
       printf("Incorrect arguments!\n");
       return 1;
    }
    
    pthread_mutex_init (&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, 3);

    int i;
    int n = atoi(argv[1]);

    pthread_t thr[n];
    THR_PARAM thr_param[n];
     
    thr_param->n = n * 2;
    
    int ARR_SIZE = thr_param->n;

    thr_param->array = (int*)malloc(ARR_SIZE * sizeof(int));

    for(i = 0; i < 2 * n; i++){
    	thr_param->array[i] = 0;
    }

    for(i = 0; i < n; i++){
    	thr_param->index = i;
 	pthread_create(&thr[i], NULL, threadFun, (void*)&thr_param[i]);
    }

    for(i = 0; i < n; i++){
       pthread_join(thr[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}
