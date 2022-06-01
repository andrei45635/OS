#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void delete_element(int* arr, int size, int pos){

    int i;

    for(i = pos - 1; i < size - 1; i++){
        arr[i] = arr[i+1];
    }

    size--;

}

void insert_element(int* arr, int size, int elem, int pos){

    int i;

    size++;

    for(i = pos - 1; i < size - 1; i++){
       arr[i] = arr[i - 1];
    }

    arr[pos - 1] = elem;

}

int main(int argc, char** argv){

    if(argc != 2){
       printf("You can only input 1 argument!\n");
       return 1;
    }

    int i, j;

    int n = atoi(argv[1]);

    int* arr = (int*)malloc(n * sizeof(int));
    char* found = (char*)malloc(n * sizeof(char));

    srand(time(NULL));

    for(i = 0; i < n; i++){
        int x = rand() % 50;
        arr[i] = x;
    }

    memset(found, '0', sizeof(char));

    int count = sizeof(arr)/sizeof(arr[0]);

    for(i = 0; i < count - 1; i++){
       for(j = i + 1; j < count; j++){
           if(arr[i] == arr[j]){
                found[i] = '1';
           }
        }
    }

    for(i = 0; i < n; i++){
        if(found[i] == '1'){
          int y = rand() % 50;
          delete_element(arr, n, i);
          insert_element(arr, n, y, i);
        }
    }

    for(i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }

   free(arr);
   free(found);

    printf("\n");
}
