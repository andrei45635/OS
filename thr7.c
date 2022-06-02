#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

typedef struct indexes{
   int start;
   int end;
   int structK;
}index;

int arr[1024];
int countTwennyOne; 
int globalCount;
int THR_COUNT;

pthread_mutex_t mutexCount;
sem_t semCount;

void* thread_fun1(void* args){
  
    srand(time(NULL));

    int i, x = 0;

    int n = 0;
    n = *(int*)args;

    for(i = 0; i < n; i++){
       x = rand() % n + 1;
       arr[i] = x;
    }

    return 0;
}

void* thread_fun(void* args){
    
    index param; 
    param = *(index*)args;

    int i;

    for(i = param.start; i < param.end; i++){
        if(arr[i] % param.structK == 0){
	   sem_wait(&semCount);
	   countTwennyOne++;
	   sem_post(&semCount);
	}
    }
    
    printf("In the interval %d -> %d, there are %d numbers divisible by K = %d\n", param.start, param.end, countTwennyOne, param.structK);
    
    pthread_mutex_lock(&mutexCount);

    globalCount += countTwennyOne;

    pthread_mutex_unlock(&mutexCount);

    countTwennyOne = 0;

    return 0;
}

int main(int argc, char** argv){

    int i;

    printf("Input the number of threads you desire: ");
    scanf("%d", &THR_COUNT);
     
    pthread_t thr[THR_COUNT];

    pthread_mutex_init(&mutexCount, NULL);
    sem_init(&semCount, 0, THR_COUNT/2);

    index indice[THR_COUNT];

    int n = 0, k = 0;

    printf("Input K (5, 7, 13, 19): ");
    scanf("%d", &k);
  
   if(k != 5 && k != 7 && k != 13 && k != 19){
       printf("Incorrect value!\n");
       return 1;
    }

    printf("Input N (200, 400, 600): ");
    scanf("%d", &n);

    if(n != 400 && n != 600 && n != 200){
       printf("Incorrect N!\n");
       return 1;
    }

    for(i = 0; i < THR_COUNT; i++){
	if(i == 0){
	   pthread_create(&thr[i], NULL, thread_fun1, (void*)&n);
	}
         else { 
            indice[i].start = n/THR_COUNT * i;
	    indice[i].end = n/THR_COUNT * (i + 1);
	    indice[i].structK = k;
            pthread_create(&thr[i], NULL, thread_fun, (void*)&indice[i]);
	 }
    }

    for(i = 0; i < THR_COUNT; i++){
        pthread_join(thr[i], NULL);
    }

    pthread_mutex_destroy(&mutexCount);
    sem_destroy(&semCount);

    printf("There are %d numbers divisible by K\n", globalCount);

    return 0;
}
