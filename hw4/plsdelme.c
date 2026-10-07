#include <stdlib.h>
#include <stdio.h>

int idx(int x, int y, int k)
 {
  //gridsize k
	  int value = y;
      int yd = !value;
	  while(value) {yd++; value/=10;};
      int result = 1;
      for(int i = 0; i < yd; i++){
          result *= 10;
      } // 10^yd
      x *= result;
      return (x + y); //index is x*10^len(y)+y ie append y to x
  }
int main(int argc, char *argv[]){
	/*if(argc != 4){
		printf("Usage: %s k x y\n", argv[0]);
		return 0;
	}*/
	int k = atoi(argv[1]);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	int output = idx(k, x, y);
	printf("%d\n", output);
	return 0;
}
