#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THR_COUNT 100
#define ARR_SIZE 100000

typedef struct _THR_PARAM{
    int* array;
    int idx_start;
    int idx_end;
}THR_PARAM;

int global_sum = 0;

pthread_mutex_t mutex;

void* sum_threads(void* param){
    THR_PARAM thr_param;

    int i;

    int local_sum = 0;
    
    thr_param = *(THR_PARAM*)param;

    for(i= thr_param.idx_start; i < thr_param.idx_end; i++){
        local_sum += thr_param.array[i];
    }
    printf("Thread %lu calculated the sum %d\n", pthread_self(), local_sum);
    pthread_mutex_lock(&mutex);

    global_sum += local_sum;

    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(int argc, char** argv){
    int i;
    int array[ARR_SIZE];
    pthread_t thr[THR_COUNT];
    THR_PARAM thr_param[THR_COUNT];
	
    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < ARR_SIZE;i++){
        array[i] = i;
    }

    for(i = 0; i < THR_COUNT; i++){
        thr_param[i].array = array;
	thr_param[i].idx_start = (ARR_SIZE/THR_COUNT) * i;
	thr_param[i].idx_end = (ARR_SIZE/THR_COUNT) * (i + 1);
    	
	pthread_create(&thr[i], NULL, sum_threads, (void*)&thr_param[i]);
    }
	
    for(i = 0; i < THR_COUNT; i++){
        pthread_join(thr[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Sum = %d\n", global_sum);
    return 0;
}
