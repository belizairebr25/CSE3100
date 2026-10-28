#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

int child1_main(int p1[2], int p2[2], int p3[2], int p4[2], int p5[2], int p6[2]){
//close unused fds
	close(p1[1]);
	close(p2[0]);
	close(p3[0]);
	close(p3[1]);
	close(p4[0]);
	close(p4[1]);
	close(p5[1]);
	close(p6[0]);
//read char 1 from pipe from parent
	char input;
	ssize_t n = read(p1[0], &input, sizeof(input));
	if (n == 1) {
		//if input has char
		printf("Child 1 read %c\n", input);
		char output = (char)((int)input + 1); //cast to int add one cast back
		printf("Child 1 wrote: %c\n", output);
		write(p2[1], &output, sizeof(char));
		close(p1[0]);
		close(p2[1]);
		//read from child 2
		n = read(p5[0], &input, sizeof(input));
		if (n == 1){
			//write back to parent
			printf("Child 1 read %c\n", input);
			output = (char)((int)input + 1);//add one
			printf("Child 1 wrote %c\n", output);
			write(p6[1], &output, sizeof(char));
			close(p6[1]);
			close(p5[0]);
			return 0;
		} else {
			perror("error on second input to child one");
			return -1;
		}
	} else {
		perror("child one received invalid input");
		return -1;
	}
}

			

int child2_main(int p1[2], int p2[2], int p3[2], int p4[2], int p5[2]){
//close unused fds
	close(p1[0]);
	close(p1[1]);
	close(p2[1]);
	close(p3[0]);
	close(p4[1]);
	close(p5[0]);
//read char from child 1
	char input;
	ssize_t n = read(p2[0], &input, sizeof(input));
	if (n == 1) {
		//if input has char
		printf("Child 2 read %c\n", input);
		char output = (char)((int)input + 1); //cast to int add one cast back
		printf("Child 2 wrote: %c\n", output);
		close(p2[0]);
		//write to parent
		write(p3[1], &output, sizeof(char));
		close(p3[1]);
		//read from parent
		n = read(p4[0], &input, sizeof(input));
		if (n == 1){
			//write back to child 1
			printf("Child 2 read: %c\n", input);
			output = (char)((int)input + 1); //add one
			printf("Child 2 wrote: %c\n", output);
			write(p5[1], &output, sizeof(char));
			close(p5[1]);
			close(p4[0]);
			return 0;
		} else {
			perror("error on second input to child 2");
		}
	} else {
		perror("child two received invalid input");
		return -1;
	}
}

int main(int argc, char **argv){
	//check inputs
	if(argc < 1){
		printf("usage: %s c\n", argv[0]);		
	}
	//make pipes
	int p1[2], p2[2], p3[2], p4[2], p5[2], p6[2];
	if(pipe(p1) == -1 || pipe(p2) == -1 || pipe(p3) == -1 || pipe(p4) == -1 || pipe(p5) == -1 || pipe(p6) == -1){
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
			child2_main(p1, p2, p3, p4, p5);
		} else if(pid > 0){
			//child1 parents child2
			child1_main(p1, p2, p3, p4, p5, p6);
		}	
	} else if (pid > 0){
		//parent: send first char to child; wait for children and terminate
		close(p2[0]);
		close(p2[1]);
		close(p1[0]);
		close(p3[1]);
		close(p4[0]);
		close(p5[0]);
		close(p5[1]);
		close(p6[1]);
		char output = argv[1][0];
		write(p1[1], &output, sizeof(char));//first write
		close(p1[1]);
		char input;
		//read from child 2
		ssize_t n = read(p3[0], &input, sizeof(input));
		if(n == 1){
			printf("Parent read: %c\n", input);
			output = (char)((int)input + 1);//add one
			printf("Parent wrote: %c\n", output);
			write(p4[1], &output, sizeof(char));
			close(p4[1]);
			close(p3[0]);
			//read from child 1
			n = read(p6[0], &input, sizeof(input));
			close(p6[0]);
			if(n == 1){
				printf("Parent read: %c\n", input);
				output = (char)((int)input + 1); //add one
				printf("Parent wrote: %c\n", output);
			}
		wait(NULL);//wait for children before terminating
		}
	return 0;
	}
}
