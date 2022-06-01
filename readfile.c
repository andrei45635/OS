#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LEN 100

typedef struct{
   char operand[MAX_LEN];
   int op1;
   int op2;
   int filesize;
   FILE* pt;
}oper;

int main(int argc, char** argv){
    int a2b[2];
    oper operations;
    if(pipe(a2b) == -1){
      printf("Error when creating pipe\n");
      return 1;
    }
    if(fork() == 0) { 
	close(a2b[1]);
	double rez;
	while(1){
	if(read(a2b[0], &operations, sizeof(oper)) == -1){
	   printf("Error when reading from A\n");
	   return 1;
	}
	if(strcmp(operations.operand, "*") == 0){
	   rez = operations.op1 * operations.op2;
	   printf("%d %s %d = %.2f\n", operations.op1, operations.operand, operations.op2, rez);
	}
	if(strcmp(operations.operand, "+") == 0){
	   rez = operations.op1 + operations.op2;
	   printf("%d %s %d = %.2f\n", operations.op1, operations.operand, operations.op2, rez);
	}
	if(strcmp(operations.operand, "/") == 0){
	   rez = operations.op1 / operations.op2;
	   printf("%d %s %d = %.2f\n", operations.op1, operations.operand, operations.op2, rez);
	}
	if(strcmp(operations.operand, "-") == 0){
	  rez = operations.op1 - operations.op2;
	  printf("%d %s %d = %.2f\n", operations.op1, operations.operand, operations.op2, rez);
	}
	if(!feof(operations.pt)){
	   close(a2b[0]);
	   exit(0);
	}
      }
    } else {
    	close(a2b[0]);
	FILE* ptr = fopen("operations.txt", "r");
	operations.pt = ptr;
    	if(ptr == NULL){
      		printf("File can't be opened\n");
      		return 1;
    	}
    	while(!feof(ptr)){
       		fscanf(ptr, "%s %d %d", operations.operand, &operations.op1, &operations.op2);
	        if(write(a2b[1], &operations, sizeof(oper)) == -1){
		  printf("Error when writing to B\n");
		  return 1;
		}
    	}
	 close(a2b[1]);
   	 wait(0);
    }
    return 0;
}
