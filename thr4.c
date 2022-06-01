#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct{
    char fileName[64];
    char line[1024];
    char key;
}THR_PARAM;

void* threadFun(void* arg){

    THR_PARAM param = *(THR_PARAM*)arg;
	
    char* buffer = (char*)malloc(1024 * sizeof(char));

    FILE* fpthr = fopen(param.fileName, "w");

    int i;

    fseek(fpthr, 0, SEEK_SET); //seek to the beginning of the file

    fread(buffer, strlen(param.line) + 1, 1, fpthr); //in buffer ar trb sa avem textul din fisier

    for(i = 0; i < strlen(buffer); i++){
        buffer[i] = buffer[i] ^ param.key;
    }

    strcat(param.fileName, ".crpyt");

    FILE* fpthr1 = fopen(param.fileName, "w");

    fwrite(buffer, strlen(buffer) + 1, 1, fpthr1);

    free(buffer);

    fclose(fpthr);
    fclose(fpthr1);

    return 0;
}

int main(int argc, char** argv){
    
    pthread_t thr;

    THR_PARAM thr_param;

    printf("Input the file name: ");
    scanf("%s", thr_param.fileName);
    printf("Input the encryption key: ");
    scanf("%c", &thr_param.key);
    
    pthread_create(&thr, NULL, threadFun, (void*)&thr_param);

    pthread_join(thr, NULL);

    FILE* fptr = fopen(thr_param.fileName, "r");

    while(fscanf(fptr, "%s", thr_param.line) == 1){
    	printf("%s", thr_param.line);
    }

    fclose(fptr);

    return 0;
}
