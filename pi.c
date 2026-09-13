#include <stdio.h>
#include <stdlib.h>
int exponentiator(int value, int exponent){
	if (exponent == 0){
		return 1;
	}
	for(int i = 0; i < exponent; i++){
		value *= value;
	}
	return value;
}
double pi_series(double n){
	double result;
	double iter;
	for(int i = 0; i < n; i++){
		iter += ( (4 / (8 * i + 1)) - (1 / (8 * i + 4)) - (1 / (8 * i + 5)) - (1 / (8 * i + 6)) ) * (1 / exponentiator(16, i));
		result += iter; //aggregate result of series iteration
	}
	return result;
}
int main(){
	int n;
	double result;
	printf("n = ");
	scanf("%d", &n);
	result = pi_series(n);
	printf("PI = %.10f\n", result);
	return 0;
}
