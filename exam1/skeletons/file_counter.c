#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
/* FIXED: <fcntl.h> was missing; open() and O_RDONLY are declared there.
 *        <stdint.h> added for the real SIZE_MAX (see the #define note below). */
#include <fcntl.h>
#include <stdint.h>
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
/* FIXED: removed "#define SIZE_MAX 2048". SIZE_MAX is a standard macro from
 * <stdint.h>; redefining it is undefined behavior/a warning, and with the value
 * 2048 the overflow check in read_file_to_array() (capacity > SIZE_MAX/2 - 1)
 * would reject any file that outgrew the initial 4096-byte buffer. */

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

/* ADDED: helpers. A single read()/write() on a pipe may transfer fewer bytes
 * than requested (pipe buffer is ~64KB, and signals can interrupt), so
 * "ensures child receives whole file" requires looping. Return 0 on success,
 * -1 on error or premature EOF. */
static int read_all(int fd, void *buf, size_t n)
{
    unsigned char *p = buf;
    while (n > 0) {
        ssize_t r = read(fd, p, n);
        if (r > 0) { p += r; n -= (size_t)r; }
        else if (r == 0) return -1;              /* EOF before all bytes arrived */
        else if (errno == EINTR) continue;
        else return -1;
    }
    return 0;
}

static int write_all(int fd, const void *buf, size_t n)
{
    const unsigned char *p = buf;
    while (n > 0) {
        ssize_t w = write(fd, p, n);
        if (w >= 0) { p += w; n -= (size_t)w; }
        else if (errno == EINTR) continue;
        else return -1;
    }
    return 0;
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

/* ---------------------------------------------------------------------------
 * Bugs shared by all three child functions (fixed in each):
 *
 *  1. The parent sent the *pointer* to the file contents through the pipe.
 *     Processes have separate address spaces, and the children were forked
 *     BEFORE the file was read, so that address means nothing to them
 *     (reading through it is a segfault/garbage). The actual bytes must be sent.
 *     Protocol is now: length (size_t), then <length> bytes of contents.
 *     (Length goes first so the child knows how much to allocate/read.)
 *  2. "if(n != 1)": read() returns a byte count, so it can never be 1 for a
 *     pointer/size_t read. Compare against the expected size instead (done by
 *     read_all()).
 *  3. `int count;` was never initialized, so counting started from garbage.
 *  4. Final "close(cXtop[WRITE], &count, sizeof(count))" passed 3 args to
 *     close(); that does not compile. It should just be close(cXtop[WRITE]).
 *  5. The functions returned to main(), so a child would fall through and
 *     execute the parent/other-child code. Children must exit().
 * ------------------------------------------------------------------------- */
int child1_main(int ptoc1[2], int c1top[2]){
	//close fds
	close(ptoc1[WRITE]);
	close(c1top[READ]);
	//receive file
	unsigned char *contents;
	size_t length;
	int n;
	n = read_all(ptoc1[READ], &length, sizeof(length)); //read length (FIXED: now first)
	if(n != 0)
		die("child one failed to receive length from parent");
	contents = malloc(length + 1); //FIXED: need our own buffer to hold the bytes
	if(contents == NULL)
		die("child one malloc fail");
	n = read_all(ptoc1[READ], contents, length); //read contents (FIXED: bytes, not a pointer)
	if(n != 0)
		die("child one failed to receive contents from parent");

	close(ptoc1[READ]); //done reading

	//report back
	write_all(c1top[WRITE], &length, sizeof(length));
	
	//linear char search
	int count = 0; //FIXED: was uninitialized
	int len = (int)length; //cast length to int
	for(int i = 0; i < len; i++){
		/* FIXED: (int)"\n" is the ADDRESS of a string literal, so this never
		 * matched. Compare against the char constant '\n'. */
		if(contents[i] == '\n')
			count++;
	}

	//write back
	write_all(c1top[WRITE], &count, sizeof(count));
	close(c1top[WRITE]); //FIXED: close() takes one argument
	exit(EXIT_SUCCESS); //FIXED: child must exit, not return into main()
}

int child2_main(int ptoc2[2], int c2top[2]){
    //close fds
    close(ptoc2[WRITE]);
    close(c2top[READ]);
    //receive file (same protocol fixes as child 1)
    unsigned char *contents;
    size_t length;
    int n;
    n = read_all(ptoc2[READ], &length, sizeof(length)); //read length
    if(n != 0)
        die("child two failed to receive length from parent");
    contents = malloc(length + 1);
    if(contents == NULL)
        die("child two malloc fail");
    n = read_all(ptoc2[READ], contents, length); //read contents
    if(n != 0)
        die("child two failed to receive contents from parent");

    close(ptoc2[READ]); //done reading

    //report back
    write_all(c2top[WRITE], &length, sizeof(length));
    
    //linear char search
    int count = 0; //FIXED: was uninitialized
    int len = (int)length; //cast length to int
    for(int i = 0; i < len; i++){
		int val = (int)contents[i];
        if((val >= 65 && val <= 90) || (val >= 97 && val <= 122))
            count++;
    }

    //write back
    write_all(c2top[WRITE], &count, sizeof(count));
    close(c2top[WRITE]); //FIXED: close() takes one argument
    exit(EXIT_SUCCESS); //FIXED: child must exit
}

int child3_main(int ptoc3[2], int c3top[2]){
    //close fds
    close(ptoc3[WRITE]);
    close(c3top[READ]);
    //receive file (same protocol fixes as child 1)
    unsigned char *contents;
    size_t length;
    int n;
    n = read_all(ptoc3[READ], &length, sizeof(length)); //read length
    if(n != 0)
        die("child three failed to receive length from parent");
    contents = malloc(length + 1);
    if(contents == NULL)
        die("child three malloc fail");
    n = read_all(ptoc3[READ], contents, length); //read contents
    if(n != 0)
        die("child three failed to receive contents from parent");

    close(ptoc3[READ]); //done reading

    //report back
    /* FIXED: was writing to c1top, which isn't even a parameter of this
     * function (compile error), and would have been the wrong pipe anyway. */
    write_all(c3top[WRITE], &length, sizeof(length));
    
    //linear char search
    int count = 0; //FIXED: was uninitialized
    int len = (int)length; //cast length to int
    for(int i = 0; i < len; i++){
		int val = (int)contents[i];
        if(val >= 48 && val <= 57)
            count++;
    }

    //write back
    write_all(c3top[WRITE], &count, sizeof(count));
    close(c3top[WRITE]); //FIXED: close() takes one argument
    exit(EXIT_SUCCESS); //FIXED: child must exit
}

int main(int argc, char **argv){
	//check input
	/* FIXED: was "argc != 1", which rejects the normal invocation
	 * (prog file => argc == 2) and would let argv[1] be read as NULL. */
	if(argc != 2)
		die("Incorrect number of args");
	//open input file
	FILE *fp = fopen(argv[1], "r");
	if (!fp){
		die("could not open file");
	}
	fclose(fp); //FIXED: handle was leaked; read_file_to_array() reopens the file itself
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
		child1_main(ptoc1, c1top); //FIXED: missing semicolon
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
	/* FIXED: function is named read_file_to_array (file_to_array doesn't
	 * exist), and its failure return was ignored. */
	if(read_file_to_array(argv[1], &contents, &length) == -1)
		die("could not read file");
	
	//send file to children
	/* FIXED: was writing &contents (the pointer variable) and sizeof(pointer).
	 * Send length first, then the actual bytes, looping until all are written. */
	if(write_all(ptoc1[WRITE], &length, sizeof(length)) == -1 ||
	   write_all(ptoc1[WRITE], contents, length) == -1)
		die("write to child 1 error");

	if(write_all(ptoc2[WRITE], &length, sizeof(length)) == -1 ||
	   write_all(ptoc2[WRITE], contents, length) == -1)
		die("write to child 2 error");

	if(write_all(ptoc3[WRITE], &length, sizeof(length)) == -1 ||
	   write_all(ptoc3[WRITE], contents, length) == -1)
		die("write to child 3 error");

	//check if children received file
	size_t input; //size of file
	/* FIXED (all reads below): "n != 1" -> use read_all and check for 0. */
	int n = read_all(c1top[READ], &input, sizeof(input));
	if(n != 0)
		die("read from child 1 error");
	if(input != length)
		die("child one cannot read from file");
	n = read_all(c2top[READ], &input, sizeof(input));
	if(n != 0)
		die("read from child 2 error");
	if(input != length)
		die("child 2 cannot read from file");
	n = read_all(c3top[READ], &input, sizeof(input));
	if(n != 0)
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
	n = read_all(c1top[READ], &in1, sizeof(in1));
	if(n != 0)
		die("read from child 1 error");
	n = read_all(c2top[READ], &in2, sizeof(in2));
	if(n != 0)
		die("read from child 2 error");
	n = read_all(c3top[READ], &in3, sizeof(in3));
	if(n != 0) //FIXED: this result was never checked, so in3 could be garbage
		die("read from child 3 error");
	
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
