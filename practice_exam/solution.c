#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//parent -> three children
//parent collects results from children with pipes
//parent opens input file
//creates three child processes
//ensures child receives whole file

//child one counts newline chars
//child two counts alphabetic chars
//child 3 counts decimal digits
#define READ 0
#define WRITE 1
void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
} 

int main(int argc, char **argv){
	//check input
	if(argc != 1)
		die("Incorrect number of args");
	//open input file
	FILE *fp = fopen(argv[1], "r");
	if (!fp){
		die("could not open file");
	}
	//setup pipes
	int ptoc1[2], ptoc2[2], ptoc3[2], c1top[2], c2top[2], c3top[2];
	if(pipe(ptoc1) == -1 || pipe(ptoc2) == -1 || pipe(ptoc3) == -1 || pipe(c3top) == -1 || pipe(c2top) == -1 || pipe(c1top) == -1)
		die("pipe() fail");
	//create 3 children
	pid_t pid;
	pid = fork(); //make child 1
	if(pid < 0)
		die("fork() fail");
	if(pid == 0){
		pid = fork(); //make child 2
		if(pid < 0)
			die("fork() fail");
		if(pid == 0){
			pid = fork(); //make child 3
			if(pid < 0)
				die("fork() fail");
			if(pid == 0){
				//child 3
				child3_main(ptoc3, c3top);
			}
			//child2
			child2_main(ptoc2, c2top);
		}
		//child1
		child1_main(ptoc1, c1top)
	}
	//parent
	//close unused file descriptors
	close(c1top[WRITE]);
	close(c2top[WRITE]);
	close(c3top[WRITE]);

	close(ptoc1[READ]);
	close(ptoc2[READ]);
	close(ptoc3[READ]);
	//send file contents to children
	//read file to array
	char *file_array[MAX_FILE_LINES][CHARS];
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread;
	size_t line_number = 0;
	while((nread = getline(&line, &cap, fp)) != -1){
		line_number++;
		file_array[i] = line;
		(void)nread;
	}

	//check if children received file
	int input; //0 if received, 1 if failed
	ssize_t n = read(c1top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 1 error");
	if(input != 0)
		die("child one cannot read from file");
	n = read(c2top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 2 error");
	if(input != 0)
		die("child 2 cannot read from file");
	n = read(c3top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 3 error");
	if(input != 0)
		die("child 3 cannot read from file");
	
	//get counts from children
	int in1;
	int in2;
	int in3;	
}






