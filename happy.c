#include <stdio.h>
#define max_input_length 8
void parser(int n){
	//use mathematical parsing formula digit = (x//10^n) % 10 for digit selection
	int digits[max_input_length] = {};
	int power = 1;
	for(int i = 0; i < max_input_length; i++){
		digits[max_input_length - 1 - i] = (n / power) % 10;
		power *= 10;
					
	}
	for(int i = 0; i <max_input_length; i++){
		printf("%d", digits[i]);
	}
	printf("\n");
}
int main(){
	int n;
	printf("n = ");
	scanf("%d", &n);
	parser(n);
	return 0;
}
