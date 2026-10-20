#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

//Using these two global variables can be justified :)
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int word_count = 0;                    //number of words, initilized to 0

//Note the words in the dictionary file are sorted
//This fact could be useful
void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
    //see how this is done using fscanf 
    while(word_count < MAX_WORD_COUNT && fscanf(fp, "%79s", words[word_count]) ==1){
	word_count++;
	}
    fclose(fp);
}
//TODO
//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int in_dict(char *word)
{
	int low = 0;
	int high = word_count - 1;	
	while (low <= high){
		int mid = low + (high - low) / 2;
		if (strcmp(words[mid], word) == 0)
			return 1;
		if (strcmp(words[mid], word) < 0)
			low = mid + 1;
		else
			high = mid - 1;
	}
	return 0;
}

//TODO
//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted

void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted)
{
	int i = 0;
	for(;i < len; i++){
		decrypted[i] = (encrypted[i] ^ key) >> shift;
	}
	decrypted[i] = '\0';

}

//TODO
//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg)
{
	int j = 0;
	int score = 0;
	//iterate through message wordwise
	while(msg[j] != '\0'){
		char s[256];
		int si = 0;
		//iterate letterwise
		while(msg[j] != '\0' && msg[j] != ' '){
			if(si < (int)sizeof(s) - 1){
				s[si++] = msg[j];
			}
			j++;		
		}
		//terminate word
		s[si] = '\0';
		if(msg[j] == ' ')
			j++;
		//check word existence
		if(in_dict(s))
			score++;
	}
	return score;
}

//search using all the (key, shift) combinations
//to find the original message
//result is saved in message
void search(const int *encrypted, int len, char *message)
{
	char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    for(unsigned char k = 0; k < 255; k++)
    {
        for(unsigned char shift = 0; shift <= 24; shift++)
        {
            decryption(k, shift, encrypted, len, decrypted);
			int score = message_score(decrypted);
			if(score > max_score)
			{	
				max_score = score;
				strcpy(message, decrypted);
			}
        }
    }
}

//TODO
//read the encrypted message from the file to encrypted
//return number of bytes read
int read_encrypted(char *filename, int *encrypted)
{
	FILE *fptr = fopen(filename, "rb");
	if (fptr == NULL){
		perror(filename);
		return -1;
	}
	int count = fread(encrypted, sizeof(int), MAX, fptr);
	fclose(fptr);
	return count;
}

//Do not change the main() function
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s encrypted-message\n", argv[0]);
		return 0;
	}

	read_file_to_array("dict.txt");

	int encrypted[MAX];
	int len = read_encrypted(argv[1], encrypted);
	
	char message[MAX];
	strcpy(message, "");
	search(encrypted, len, message);
	printf("%s\n", message);
	return 0;
}
