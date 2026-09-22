#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 65536

double two_d_random(int n)
{	int used[MAX_SIZE][2] = {{0, 0}};
	double total = (2 * n - 1) * (2 * n - 1);
	int count = 1;
	int x = 0; // x coordinate
	int y =0;  // y coordinate
	while(x != n && x != (-1 * n) && y != n && y != (-1 * n)){
		int found = 0;
		int r = rand()  % 4;
		if(r == 0){
			y += 1;
		} else if(r == 1){
			x += 1;
		} else if(r == 2){
			y -= 1;
		} else if(r == 3){
			x -= 1;
		}
		for(int i = 0; i < count; i++){
			if (used[i][0] == x && used[i][1] == y){
				found = 1;
				break;
			} 
		} 
		if(found != 1){
			used[count][0] = x;
			used[count][1] = y;
			count++;
		}
	}
	count -= 1;
	return count / total;
	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.





	
}

//Do not change the code below
int main(int argc, char *argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}

