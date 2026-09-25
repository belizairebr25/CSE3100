#include <stdlib.h>
#include <stdio.h>
#define SPACE 512
int main(){
	int *memy = malloc(SPACE);
	if (memy == NULL){
		printf("bruh\n");
		return 1;
	}
}
