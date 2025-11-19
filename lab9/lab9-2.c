// Lab 9 - Part 2
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define MAXWORDS 100
#define WORDLEN 11
#define DEBUG 1   // set to 0 to disable debug output

// reads words from the file
// into wl and trims the whitespace off of the end of each word
// DO NOT MODIFY THIS Prototype
// Students are REQUIRED to use fgets() to retreive each line from the file. 
int readWords(char* wl[MAXWORDS], char* filename); 

//modifies s to trim white space off the right side
// DO NOT MODIFY THIS Prototype
void trimws(char* s) ;

int main(int argc, char* argv[]) {
	char* wordlist[MAXWORDS];
	int wordCount;
	int i;
	wordCount = readWords(wordlist, argv[1]);

	if (DEBUG) {
		printf("Read %d words from %s \n",wordCount, argv[1]);

		// add code to print the words to the screen here for part 1 
		for (i = 0; i < wordCount; i++) {
			printf("%15s\0", wordlist[i]);

			if (i % 5 == 4) {
				printf("\n", wordlist[i]);
			}
		}
	}

	// most of your code for part 2 goes here. Don't forget to include the ncurses library 

	fflush(stdin);

    return 0;
}

int readWords(char* w1[MAXWORDS], char* filename) {
	int i;
	
	FILE *file_stream = fopen(filename, "r");

	while (1) {
		w1[i] = (char*)malloc(sizeof(char) * (WORDLEN + 1));

		if (!fgets(w1[i], WORDLEN, file_stream)) {
			break;
		}
		trimws(w1[i]);
		i++;
	}

	return i;
}

void trimws(char* s) {
	int i;

	for (i = 0; i < WORDLEN; i++) {
		if (s[i] == '\n' || s[i] == ' ') {
			s[i] = '\0';
			break;
		}
	}
}
