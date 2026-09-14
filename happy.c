#include <stdio.h>

int happy(int n){
	int sum = 0;
	if(n == 1 || n == 0){
		return 1;
	} else if(n == 4){
		return 0;
	}
	while (n > 0) {
		int digit = n % 10;
		sum += digit * digit;
		n /= 10;
	}
	printf("%d\n", sum);
	return happy(sum);
}
int main(){
	int n;
	printf("n = ");
	scanf("%d", &n);
	int ret = happy(n);
	if(ret == 1){
		printf("%d is a happy number.\n", n);
	} else {
		printf("%d is NOT a happy number.\n", n);
	}
	return 0;
}
