#include <stdio.h>
unsigned int v = 19;
char parity = 0;
int main(){
	while(v){
		parity = !parity;
		v = v & (v-1);
	}
}

