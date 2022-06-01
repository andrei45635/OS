//Sample downloader using cURL (libcurl). Demonstrates the usage of semaphore to limit the number of concurrent downloads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include "curl/curl.h"

static int PARALLEL_DOWNLOADS = 3;
static int THR_COUNT = 5;
static char* urls[] = {
	"https://ortodoxinfo.ro",
	"https://doxologia.ro",
	"https://crestinortodox.ro",
	"https://sufletortodox.ro",
	"https://ortodoxism.ro"
};

int nextURL = 0;
pthread_mutex_t nextURL_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t downloader_sem;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata){
    return nmemb;
}

void* downloader(void* p){
    while(1){
       int n = 0;
       
       pthread_mutex_lock(&nextURL_mutex);
       n = nextURL++;
       pthread_mutex_unlock(&nextURL_mutex);
      
       if(n > sizeof(urls)/sizeof(urls[0]) - 1) break;       
      
       sem_wait(&downloader_sem);
       printf("Thread %ld is preparing to download the url %s with the index %d\n", pthread_self(), urls[n], n);

       CURL *curl;
       CURLcode res;

       curl = curl_easy_init();

       if(curl) {
         curl_easy_setopt(curl, CURLOPT_URL, urls[n]);
	 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

	 res = curl_easy_perform(curl);

	 if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	 curl_easy_cleanup(curl);
        }
      printf("Thread %ld done with the url: %s\n", pthread_self(), urls[n]);
      sem_post(&downloader_sem);
    }
    return NULL;
}

int main(int argc, char** argv){
    
    int i;

    pthread_t thds[THR_COUNT];
    sem_init(&downloader_sem, 0, PARALLEL_DOWNLOADS);

    for(i = 0; i < THR_COUNT; i++){
        pthread_create(&thds[i], NULL, downloader, NULL);
    }

    for(i = 0; i < THR_COUNT; i++){
       pthread_join(thds[i], NULL);
    }

    sem_destroy(&downloader_sem);
    return 0;
}
