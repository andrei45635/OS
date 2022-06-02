#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

int fuel = 0;

void* fuel_tank(void* arg){
    
    int i;
    //int times = 0;
    int times = *(int*)arg;

    for(i = 0; i < times; i++){
   	pthread_mutex_lock(&mutexFuel);

	fuel += 100;
	printf("Fuelling...%d\n", fuel);
	
	pthread_mutex_unlock(&mutexFuel);
	
	//pthread_cond_signal(&condFuel);
	pthread_cond_broadcast(&condFuel);
	sleep(1);
    }

    return 0;
}

void* car(void* arg){
    
    pthread_mutex_lock(&mutexFuel);
    
    while(fuel < 40){
      printf("Not enough fuel yet... Waiting...\n");
      pthread_cond_wait(&condFuel, &mutexFuel);
    }
    fuel -= 40;
    printf("Fuelled...%d\n", fuel);
    
    pthread_mutex_unlock(&mutexFuel);

    return 0;
}

int main(int argc, char** argv){

    int n;
    double m;

    printf("Welcome to the Draganesti gas station!\n");
    
    sleep(1);
    
    printf("How much would you like to fill your gas tank?\n");
    scanf("%d", &n);
    
    printf("How many fuellers would you like?\n");
    scanf("%le", &m);

    int i;

    pthread_t thr[2];
    
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);

    for(i = 0; i < m; i++){
    	if(i == m || i == m - 1){
	   if(pthread_create(&thr[i], NULL, &fuel_tank, &n) != 0){
	   	printf("Error when creating the car thread\n");
		return 1;
	   } 
	} else {
	      if(pthread_create(&thr[i], NULL, &car, NULL) != 0){
	      	printf("Error when creating thread!\n");
		return 1;
	      }
	   }
	}

    for(i = 0; i < m; i++){
        if(pthread_join(thr[i], NULL) != 0){
	   printf("Error when joining the threads!\n");
	   return 1;
	}
    }
 
    printf("Thank you for choosing Draganesti!\n");

    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);

    return 0;
}
