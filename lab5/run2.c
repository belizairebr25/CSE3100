#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t value;
	int status;
    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }
	value = fork();
	if (value > 0){
		//parent
		//wait for first child

		if(waitpid(value, &status, 0) == -1){
			perror("waitpid()");
			return 1;
		}
		printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
		
		//make second child
		pid_t value2;
		value2 = fork();
		
		if (value2 > 0){
			//parent
			//wait for second child
			if(waitpid(value2, &status, 0) == -1){
				perror("waitpid()");	
				return 1;
			}
			printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
			
		} else if(value2 == 0){
			//child
			execvp(argv[3], &argv[3]);

			perror("execvp()");
			_exit(1);
		} else {
			perror("fork()");
			return 1;
		}

	} else if(value == 0){
		//child
		execlp(argv[1], argv[1], argv[2], NULL);

		perror("execlp()");
		_exit(1);
	
	} else {
		//error
		perror("fork()");
		return 1;
	}
    return 0;
}

