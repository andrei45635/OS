//Write a program that creates 5 threads, giving each thread a string as parameter. Each thread will count and add to the global variables v and n as follows: the number of vowels contained by the string added to v, and the number of digits contained in the string added to n.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define THR_COUNT 5
#define MAX_SIZE 1024

int v = 0;
int n = 0;

pthread_mutex_t mutex;

void* string_fun(void* str){
    int local_v = 0;
    
    int local_n = 0;

    int i;
    
    char string[MAX_SIZE];

    memcpy(string, str, sizeof(strlen(str)));	

    for(i = 0; string[i] !='\0'; i++){
    	if(string[i] == 'a' || string[i] == 'A' || string[i] == 'e' || string[i] == 'E' || string[i] == 'i' || string[i] == 'I' || string[i] == 'O' || string[i] == 'o' || string[i] == 'u' || string[i] == 'U'){
		local_v++;
	}
	if(string[i] >= '0' && string[i] <= '9'){
		local_n++;
	}
    }

    pthread_mutex_lock(&mutex);
    v += local_v;
    n += local_n;
    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(int argc, char** argv){
    int i, j;

    pthread_t thr[THR_COUNT];

    char string[MAX_SIZE];

    char input_string[MAX_SIZE];

    pthread_mutex_init(&mutex, NULL);

    if(argc > 1){
    	printf("Insufficient arguments\n");
	return 1;
    } else {
	for(j = 0; j < THR_COUNT; j++){
		printf("Input string %d ", j);
		scanf("%s", input_string);
		memcpy(string, input_string, sizeof(strlen(input_string)));
	}
    	for(i = 0; i < THR_COUNT; i++){
    		pthread_create(&thr[i], NULL, &string_fun, (void*)&string);
    	}		
    	for(i = 0; i < THR_COUNT; i++){
    		pthread_join(thr[i], NULL);
    	}
	 pthread_mutex_destroy(&mutex);
	 printf("Number of vowels %d and number of digits %d\n", v, n);
    }
    return 0;
}

