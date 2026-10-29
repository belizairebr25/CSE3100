#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	int fd = open("test.txt", O_RDWR);
	char buff[10];
	int v = read(fd, buff, 10);
	printf("%d", v);
	return 0;
}
