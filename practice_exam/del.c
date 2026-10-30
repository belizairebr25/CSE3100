#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);//redirect to file
	printf("ABC");
	return 0;
}
