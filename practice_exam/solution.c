#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
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
#define SIZE_MAX 2048
void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}
 
static int read_file_to_array(const char *path,
                              unsigned char **data_out,
                              size_t *size_out) {
    int fd = -1;
    unsigned char *data = NULL;
    size_t size = 0;
    size_t capacity = 4096;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    data = malloc(capacity + 1); /* +1 for optional text terminator */
    if (data == NULL) {
        perror("malloc");
        close(fd);
        return -1;
    }

    for (;;) {
        /* Keep one byte available for the final '\0'. */
        if (size == capacity) {
            if (capacity > SIZE_MAX / 2 - 1) {
                fprintf(stderr, "file is too large to fit in memory\n");
                free(data);
                close(fd);
                return -1;
            }

            size_t new_capacity = capacity * 2;
            unsigned char *tmp = realloc(data, new_capacity + 1);
            if (tmp == NULL) {
                perror("realloc");
                free(data);
                close(fd);
                return -1;
            }
            data = tmp;
            capacity = new_capacity;
        }

        ssize_t n = read(fd, data + size, capacity - size);
        if (n > 0) {
            size += (size_t)n;
        } else if (n == 0) {
            break; /* end of file */
        } else if (errno == EINTR) {
            continue; /* interrupted; retry */
        } else {
            perror("read");
            free(data);
            close(fd);
            return -1;
        }
    }

    data[size] = '\0';

    if (close(fd) == -1) {
        perror("close");
        free(data);
        return -1;
    }

    *data_out = data;
    *size_out = size;
    return 0;
}
int child1_main(int ptoc1[2], int c1top[2]){
	//close fds
	close(ptoc1[WRITE]);
	close(c1top[READ]);
	//receive file
	unsigned char *contents;
	size_t length;
	ssize_t n;
	n = read(ptoc1[READ], &contents, sizeof(contents)); //read contents
	if(n != 1)
		die("child one failed to receive contents from parent");
	n = read(ptoc1[READ], &length, sizeof(length)); //read length
	if(n != 1)
		die("child one failed to receive length from parent");

	close(ptoc1[READ]); //done reading

	//report back
	write(c1top[WRITE], &length, sizeof(length));
	
	//linear char search
	int count;
	int len = (int)length; //cast length to int
	for(int i = 0; i < len; i++){
		if((int)contents[i] == (int)"\n")
			count++;
	}

	//write back
	write(c1top[WRITE], &count, sizeof(count));
	close(c1top[WRITE], &count, sizeof(count));
	return 0; //could be void other than die?
}

int child2_main(int ptoc2[2], int c2top[2]){
    //close fds
    close(ptoc2[WRITE]);
    close(c2top[READ]);
    //receive file
    unsigned char *contents;
    size_t length;
    ssize_t n;
    n = read(ptoc2[READ], &contents, sizeof(contents)); //read contents
    if(n != 1)
        die("child two failed to receive contents from parent");
    n = read(ptoc2[READ], &length, sizeof(length)); //read length
    if(n != 1)
        die("child two failed to receive length from parent");

    close(ptoc2[READ]); //done reading

    //report back
    write(c2top[WRITE], &length, sizeof(length));
    
    //linear char search
    int count;
    int len = (int)length; //cast length to int
    for(int i = 0; i < len; i++){
		int val = (int)contents[i];
        if((val >= 65 && val <= 90) || (val >= 97 && val <= 122))
            count++;
    }

    //write back
    write(c2top[WRITE], &count, sizeof(count));
    close(c2top[WRITE], &count, sizeof(count));
    return 0; //could be void other than die?
}

int child3_main(int ptoc3[2], int c3top[2]){
    //close fds
    close(ptoc3[WRITE]);
    close(c3top[READ]);
    //receive file
    unsigned char *contents;
    size_t length;
    ssize_t n;
    n = read(ptoc3[READ], &contents, sizeof(contents)); //read contents
    if(n != 1)
        die("child three failed to receive contents from parent");
    n = read(ptoc3[READ], &length, sizeof(length)); //read length
    if(n != 1)
        die("child three failed to receive length from parent");

    close(ptoc3[READ]); //done reading

    //report back
    write(c1top[WRITE], &length, sizeof(length));
    
    //linear char search
    int count;
    int len = (int)length; //cast length to int
    for(int i = 0; i < len; i++){
		int val = (int)contents[i];
        if(val >= 48 && val <= 57)
            count++;
    }

    //write back
    write(c3top[WRITE], &count, sizeof(count));
    close(c3top[WRITE], &count, sizeof(count));
    return 0; //could be void other than die?
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

	//read file to array
	unsigned char *contents = NULL;
	size_t length = 0;
	file_to_array(argv[1], &contents, &length);	
	
	//send file to children
	write(ptoc1[WRITE], &contents, sizeof(contents));
	write(ptoc1[WRITE], &length, sizeof(length));

	write(ptoc2[WRITE], &contents, sizeof(contents));
	write(ptoc2[WRITE], &length, sizeof(length));

	write(ptoc3[WRITE], &contents, sizeof(contents));
	write(ptoc3[WRITE], &length, sizeof(length));

	//check if children received file
	size_t input; //size of file
	ssize_t n = read(c1top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 1 error");
	if(input != length)
		die("child one cannot read from file");
	n = read(c2top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 2 error");
	if(input != length)
		die("child 2 cannot read from file");
	n = read(c3top[READ], &input, sizeof(input));
	if(n != 1)
		die("read from child 3 error");
	if(input != length)
		die("child 3 cannot read from file");
	//since this is the last write to the children, close those fds
	close(ptoc1[WRITE]);
	close(ptoc2[WRITE]);
	close(ptoc3[WRITE]);

	//get counts from children
	int in1;
	int in2;
	int in3;
	n = read(c1top[READ], &in1, sizeof(in1));
	if(n != 1)
		die("read from child 1 error");
	n = read(c2top[READ], &in2, sizeof(in2));
	if(n != 1)
		die("read from child 2 error");
	n = read(c3top[READ], &in3, sizeof(in3));
	
	//print output
	printf("Lines: %d\n", in1);
	printf("Letters: %d\n", in2);
	printf("Digits: %d\n", in3);
	
	//close pipes and terminate
	close(c1top[READ]);
	close(c2top[READ]);
	close(c3top[READ]);
	return 0;
}






