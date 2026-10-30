#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//check for errors from things like read, pipe and fork
#define SYS_CHECK(x) do { if ((x) == -1) die_errno(#x); } while (0)
//assert x not zero
#define CHECK(x) do { if (!(x)) die(#x); } while (0)

//file descriptor macros
#define READ 0
#define WRITE 1

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

void die_errno(char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv){
	//TODO remove args from main if not required
	if(argc != 1)
		die("usage: ./main <argument>");
	
	printf("Test\n");	
	return 0;
}
