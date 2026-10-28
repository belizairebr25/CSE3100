#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int child1_main(){
//close unused fds
	close(p1[1]);//no writes
	close(p2[0]);//no reads
//read char 1 from pipe from parent
	char *input;
	read(p1[0], input, sizeof(char));
//get char 2, print it
	char output;
	output = (char)((int) *input + 1);//cast to int, add one, cast back to char
	printf("%s", (char)output);
//send char 2 to child 2
	write(p2[1], &output, sizeof(int));
	close(p1[0]);
}
int child2_main(){
//close unused fds
	close(p1[0]);
	close(p1[1]);
	close(p2[1]);//no writes
//read char2 from pipe from child 1
	char *input;
	read(p2[0], input, sizeof(char));
	output = (char)((int) *input + 1);
	printf("%s", (char)output);
	close(p2[0]);

}

int main(int argc, int **argv){
	//check inputs
	if(argc < 1){
		printf("usage: %ls c\n", argv[0]);		
	}
	//make pipes
	int p1[2], p2[2];
	if(pipe(p1) == -1 || pipe(p2) == -1){
		perror("pipe() failed.");
		return 1;
	}
	//make fork
	pid_t pid = fork();	
	if(pid == 0){
		//children
		pid = fork();
		if(pid == 0){
			//child2
			child2_main();
		} else if(pid > 0){
			//child1 parents child2
			child1_main();
		}	
	} else if (pid > 0){
		//parent: send first char to child; wait for children and terminate
		close(p2[0]);
		close(p2[1]);
		close(p1[0]);//no need to read
		char w = argv[1];
		char *w1[1] = w;
		write(p1[1], w1, sizeof(int));
		printf("%s", *w1);
	}
}
