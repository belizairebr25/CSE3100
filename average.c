#include <stdio.h>
double num1;
double num2;
double total;
double count;
int main(){
	count = 1;
	scanf("%lf", &num2);
	printf("Total=%lf Average=%lf\n", num2, num2);
	total = num2;
	while(scanf("%lf", &num1) == 1){
		count++;
		num2 = (total + num1) / count;
		total += num1;
		printf("Total=%lf Average=%lf\n", total, num2);
	}
	return 0;
}
