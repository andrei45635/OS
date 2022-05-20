#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char** argv){
    int p2c[2], c2p[2];
    if(pipe(p2c) == -1){
       printf("Error when creating the pipe from the parent to the child!\n");
       return 1;
    }
      if(pipe(c2p) == -1){
       printf("Error when creating the pipe from the child to the parent!\n");
       return 1;
    }
    int pid = fork();
    if(pid == -1){
       printf("Error when forking!\n");
       return 1;
    } else if(pid == 0){ //child
     	close(p2c[1]);
	close(c2p[0]);
	char sgn;
	srand(time(NULL));
	int nr = (rand() % 10000) - 999;
	if(write(c2p[1], &nr, sizeof(int)) == -1){
	  printf("Error when writing to the parent!\n");
	  return 1;
	}
	while(1){
	  if(read(p2c[0], &sgn, sizeof(char)) == -1){
	     printf("Error when reading from the parent!\n");
	     return 1;
	  } 
	  if(sgn == '<'){
	    nr += rand() % 1000;
	    printf("The parent sent %c and the child guessed %d\n", sgn, nr);
	    if(write(c2p[1], &nr, sizeof(int)) == -1){
	       printf("Error when writing to the parent from the child!\n");
	       return 1;
	    }
	  } else if(sgn == '>'){
	     nr -= rand() % 1000;
	     printf("The parent sent %c and the child guessed %d\n", sgn, nr);
	     if(write(c2p[1], &nr, sizeof(int)) == -1){
	       printf("Error when writing to the parent from the child!\n");
	       return 1;
	     } else if(sgn == '=') {
	        printf("I got it! The number was: %d!\n", nr);
		close(p2c[0]);
		close(c2p[1]);
		break;
	     }
	  }
	}
    } else { //parent
       close(p2c[0]);
       close(c2p[1]);
       srand(time(NULL));
       char sgn;
       int x = (rand() % 10000) - 1000;
       int nr;
       printf("The number that the parent thought of is: %d\n", x);
       while(1){
	  if(read(c2p[0], &nr, sizeof(int)) == -1){
	     printf("Error when reading from the child!\n");
	     return 1;
	  }
	  if(x > nr){
	     sgn = '<';
	     printf("The child guessed %d, the parent sent %c\n", nr, sgn); 
	     if(write(p2c[1], &sgn, sizeof(char)) == -1){
	       printf("Error when writing to the child!\n");
	       return 1;
	     }
	  } else if(x < nr){
	      sgn = '>';
	      printf("The child guessed %d, the parent sent %c\n", nr, sgn); 
	      if(write(p2c[1], &sgn, sizeof(char)) == -1){
	       printf("Error when writing to the child!\n");
	       return 1;
	     }
	  } else if(x == nr){
	     sgn = '=';
	     printf("The child finally guessed the number!\n");
	     if(write(p2c[1], &sgn, sizeof(char)) == -1){
	       printf("Error when writing to the child!\n");
	       return 1;
	     }
	     close(p2c[1]);
	     close(c2p[0]);
	     break;
	  }
       }
    }
    return 0;
}
