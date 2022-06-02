#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define THR_COUNT 5

int arr[1024];

int counter = 0;

pthread_mutex_t mutexArr;
pthread_cond_t condArr;

typedef struct params{
    int start;
    int end;
}THR_PARAM;

void* thread_fun(void* args){

    int x = 0, i, j, k;

    THR_PARAM param = *(THR_PARAM*)args;

    srand(time(NULL));

    for(i = param.start; i < param.end; i++){
      x = rand() % 1000 + 1;
      pthread_mutex_lock(&mutexArr);
      counter++;
      if(counter == 10) {
	 pthread_cond_broadcast(&condArr);
         counter = 0;
      }
      arr[i] = x;
      for(j = 0; j < 999; j++)
	for(k = j; k < 1000; k++){
	   if(arr[j] > arr[k]){
		int aux = arr[j];
                arr[j] = arr[k];
                arr[k] = aux;		
	   }
        }
     }
      pthread_mutex_unlock(&mutexArr);
    
    return 0;
}

int main(int argc, char** argv){

    int i;

    THR_PARAM thr_param[THR_COUNT];

    pthread_t thr[THR_COUNT];
    
    pthread_mutex_init(&mutexArr, NULL);
    pthread_cond_init(&condArr, NULL);

    for(i = 0; i < THR_COUNT; i++){
	thr_param->start = 1000/THR_COUNT * i;
	thr_param->end = 1000/THR_COUNT * (i + 1);
        pthread_create(&thr[i], NULL, &thread_fun, (void*)&thr_param[i]);
    }

    while(counter == 10){
        for(i = 0; i < 1000; i++){
	   printf("%d ", arr[i]);
	}
	pthread_cond_wait(&condArr, &mutexArr);
    }
    
    printf("\n");

    for(i = 0; i < THR_COUNT; i++){
        pthread_join(thr[i], NULL);
    }

    pthread_mutex_destroy(&mutexArr);
    pthread_cond_destroy(&condArr);

    return 0;
}
