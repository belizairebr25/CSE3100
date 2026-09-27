#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(int argc, char **argv, char *s)
{
    // TODO add chars to array
	int k = 0;
	for (int i = 0; i < argc; i++){
		for(int j = 0; j < strlen(argv[i]); j++){
			s = realloc(s, (k+1));
			s[k] = argv[i][j];
			k++;
		}
	}
	s = realloc(s, k + 1);
	s[k] = '\0';
	return s;
}

int main(int argc, char *argv[])
{
	char *s = malloc(1*sizeof(char)); //make a one character array
	if(s == NULL){
		return 1;
	}
	s[0] = 0;

	s = my_strcat(argc, argv, s);
	printf ("%s\n", s);
	free(s);
    return 0;

}
