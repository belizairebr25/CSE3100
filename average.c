#include <stdio.h>
double num1;
double num2;
double total;
int main(){
	scanf("%lf", &num2);
	printf("Total=1 Average=%lf\n", num2);
	total = 0;
	while(scanf("%lf", &num1) == 1){
		num2 = (num2 + num1) / 2;
		total += num1;
		printf("Total=%lf Average=%lf\n", total, num2);
	}
	return 0;
}
