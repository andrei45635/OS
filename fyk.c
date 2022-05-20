#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
    int p2a[2], a2p[2], p2i[2], i2p[2];
    int nra = 0, nri = 0, i;
    char stra[100] = "", stri[100] = "";
    if(argc == 1){
       printf("Insufficient arguments\n");
       return 1;
    }
    for(i = 0; i < argc; i++) {
	pipe(p2a);
	pipe(a2p);
	pipe(p2i);
	pipe(i2p);
    	if(fork() == 0){
       	    if(fork() == 0){// proc I
               close(p2a[0]);
	       close(a2p[0]);
	       close(p2i[1]);
	       close(i2p[0]);
	       close(p2a[1]);
	       close(a2p[1]);
               dup2(p2i[0], 0);
	       dup2(i2p[1], 1);
	       execlp("grep", "grep", "-c", "i", NULL);
	       close(p2i[0]);
	       close(i2p[1]);
	       exit(1):
	    } // proc A
	       close(p2i[0]);
	       close(i2p[0]);
	       close(p2i[1]);
	       close(i2p[0]);
	       close(p2a[1]);
	       close(a2p[0]);
               dup2(p2a[0], 0);
	       dup2(a2p[1], 1);
	       execlp("grep", "grep", "-c", "a", NULL);
	       close(p2a[0]);
	       close(a2p[1]);
	       exit(1);
    	}// proc P
	       char c;
	       int n;
	       close(p2a[0]);
	       close(a2p[1]);
	       close(p2i[1]);
	       close(i2p[1]);
	       write(p2a[1], argv[i], (strlen(argv[i]) * sizeof(char)));
	       close(p2a[1]);
	       read(a2p[0], &c, sizeof(char));
	       close(a2p[0]);
               n = atoi(&c);
	       if(n > 0){
	          nra++;
		  strcat(stra, argv[i]);
		  strcat(stra, " ");
	       }
	       write(p2i[1], argv[i], (strlen(argv[i]) * sizeof(char)));
	       close(p2i[1]);
	       read(i2p[0], &c, sizeof(char));
	       close(i2p[0]);
               n = atoi(&c);
	       if(n > 0){
	          nri++;
		  strcat(stri, argv[i]);
		  strcat(stri, " ");
	       }
	       printf("nra: %d, nri: %d\n", nra, nri);
	       printf("a words: %s\n", stra);
	       printf("i words: %s\n", stri);
    }
}
    return 0;
}

