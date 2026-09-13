#include <stdio.h>
#include <stdlib.h>
/*double exponentiator(double value, double exponent){
	double result = 1.0;
	if (exponent == 0){
		return 1;
	}
	for(double i = 0; i < exponent; i++){
		result *= value;
	}
	return result;
}*/
double pi_series(double n){
	double result = 0.0;
	double power = 1;
	for(double i = 0; i < n; i++){
		result += ( (4.0 / (8 * i + 1)) - (2.0 / (8 * i + 4)) - (1.0 / (8 * i + 5)) - (1.0 / (8 * i + 6)) ) * (1.0 / power);
	power *= 16;
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
