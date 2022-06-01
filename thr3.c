#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//#define ARR_SIZE 2000000000

//#define ARR_SIZE 2000

pthread_mutex_t mutex;

typedef struct{
   unsigned long long start;
   unsigned long long end;
}THR_PARAM;

unsigned long long ARR_SIZE;

int n;

unsigned long long prime_no = 0;

int isPrime(unsigned long long val){
    int ok = 1, d = 2;
    
    for(d = 2; d < val/2; d++){
    	if(val % d == 0){
	  ok = 0;
	}
    }

    if(ok) return ok;
    else return ok;
}

void* threadFun(void* param){
    
    THR_PARAM thr_param = *(THR_PARAM*)param;

    int i;

    time_t t1 = time(NULL);

    for(i = thr_param.start; i < thr_param.end; i++){
    	if(isPrime(i)){
	   pthread_mutex_lock(&mutex);
	   prime_no++;
	   pthread_mutex_unlock(&mutex);
	}
    }

    time_t t2 = time(NULL);

    double x = difftime(t2,t1);

    printf("Thread %lu has been running for %.0f seconds\n", pthread_self(),  x);

    return 0;
}

int main(int argc, char** argv){

    printf("Input the number of threads you desire (2,4,6,8): ");
    scanf("%d", &n);
    
    printf("Input the array size: ");
    scanf("%llu", &ARR_SIZE);

    pthread_t thr[n];

    THR_PARAM thr_param[n];

    int i;

    for(i = 0; i < n; i++){
    	thr_param[i].start = (ARR_SIZE / n) * i;
	thr_param[i].end = (ARR_SIZE / n) * (i + 1);
        pthread_create(&thr[i], NULL, threadFun, (void*)&thr_param[i]);	
    }

    for(i = 0; i < n; i++){
    	pthread_join(thr[i], NULL);
    }
    
    printf("Prime numbers: %llu\n", prime_no);

    return 0;
}
