/*-----------------------------------------------------------------------------
-					    CprE 1850 Lab 09 - Part 2
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-	                            Includes
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses.h>
// #include <ncurses/ncurses.h>

/*-----------------------------------------------------------------------------
-	                            Defines
-----------------------------------------------------------------------------*/
#define MAXWORDS 100
#define WORDLEN 11

#define WORDSPACING 15
#define MAXCOLS 5
#define LEFT_PADDING 1 		// Minimum 1 for selection cursor

#define MAXOUTPUTLEN 80

#define DEBUG 0   // set to 0 to disable debug output

#define JOYSTICK_THRESHOLD 0.4

typedef struct {
    int tri;	// Triangle
    int cir;	// Circle
    int x;		// Cross
    int sqr;	// Square
} ButtonState;

typedef struct {
    double x;
    double y;
    int button;
} JoystickState;


/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/
int readWords(char* wl[MAXWORDS], char* filename); 
void trimws(char* s) ;

void draw_words(char* wordlist[MAXWORDS], int wordCount);
void printOutput(char* outputArr[MAXOUTPUTLEN], int length);
void draw_character(int x, int y, char use);


/*-----------------------------------------------------------------------------
-							  Implementation
-----------------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	char* wordlist[MAXWORDS];
	int wordCount;
	int i;

	wordCount = readWords(wordlist, argv[1]);

	if (DEBUG) {
		printf("Read %d words from %s \n",wordCount, argv[1]);

		// add code to print the words to the screen here for part 1 
		for (i = 0; i < wordCount; i++) {
			printf("%s\n", wordlist[i]);
		}
		// If we're debugging, just exit after printing the words
		return 0;
	}

	// most of your code for part 2 goes here. Don't forget to include the ncurses library 
	char* outputArr[MAXOUTPUTLEN];
	int outI = 0;		// Output index in outputArr[]
	int outLen = 0;		// Current length of outputArr string

	// Current selection index
	int sel = 0;
	int selR, selC = 0;	// Selection row and column helper variables

	char* selWord;
	
	// For use when we need to capitalize a word, but don't want to modify the original
	char* tempWord = malloc(sizeof(char) * (WORDLEN + 1));

	// Capitalize next word flag
	int capNext = 0;

	// Variables for reading ds4rd.exe input
	int t = 0;
	ButtonState bState;
	JoystickState jState;
	double jx, jy = 0.0;

	initscr();
	refresh();

	draw_words(wordlist, wordCount);

	int numRows = wordCount / MAXCOLS;
	if (wordCount % MAXCOLS != 0) {
		numRows += 1;
	}

	// Main loop
	while (1) {
		// Update selection index
		selR = sel / MAXCOLS;
		selC = sel % MAXCOLS;

		// Erase previous selection cursor
		draw_character(selR * WORDSPACING + LEFT_PADDING - 1, selC, ' ');


		// -------------------- Read ds4rd.exe Input --------------------

		// ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -b -j -bt
		// time, b1, b2, b3, b4, jx, jy, j-button
		scanf(
			"%d, %d, %d, %d, %d, %lf, %lf, %d", 
			&t, 
			&bState.tri, &bState.cir, &bState.x, &bState.sqr, 
			&jState.x, &jState.y, &jState.button
		);


		// -------------------- Joystick Handling --------------------

		// Allow user to move selection cursor with joystick in the x direction
		// and y direction.  Wrap around when reaching the edges of the screen.
		if (jState.x > JOYSTICK_THRESHOLD && sel < wordCount - 1) {
			sel++;
			selR = sel / MAXCOLS;
			selC = sel % MAXCOLS;
		} 
		// If the joystick is moved left, move selection left
		else if (jState.x < -JOYSTICK_THRESHOLD && sel > 0) {
			sel--;
			selR = sel / MAXCOLS;
			selC = sel % MAXCOLS;
		}
		// Otherwise if the joystick is moved down, clear output
		else if (jState.y < -JOYSTICK_THRESHOLD) {
			outI = 0;
			outLen = 0;
		}

		// Display the selection cursor
		draw_character(selC * WORDSPACING, LEFT_PADDING + selR - 1, '>');


		// -------------------- Button Handling --------------------

		// selWord points to the currently selected word
		selWord = wordlist[selR * MAXCOLS + selC];

		// When triangle button is pressed, 
		// add a space along with the currently selected word to the outputArr
		if (bState.tri) {
			// Make sure we don't overflow outputArr, 
			// or exceed MAXOUTPUTLEN when the output string is built in printOutput()
			// +1 for space
			if (outLen + strlen(selWord) + 1 < MAXOUTPUTLEN) {
				if (capNext) {
					strcpy(tempWord, selWord);	// Copy selected word to tempWord
					
					// Capitalize the first letter of selWord
					tempWord[0] = toupper(selWord[0]);
					capNext = 0;	// Reset capitalize flag
				}

				char* str = " ";
				strcat(str, selWord);
				outputArr[outI] = str;

				// Increment output index and length
				outI++;
				outLen += strlen(str); // +1 for space
			}
		}

		// When square button is pressed, 
		// add the currently selected word to the outputArr string
		if (bState.sqr) {
			// Make sure we don't overflow outputArr, 
			// or exceed MAXOUTPUTLEN when the output string is built in printOutput()
			if (outLen + strlen(selWord) < MAXOUTPUTLEN) {
				if (capNext) {
					strcpy(tempWord, selWord);	// Copy selected word to tempWord
					
					// Capitalize the first letter of selWord
					tempWord[0] = toupper(selWord[0]);
					capNext = 0;	// Reset capitalize flag
				}

				outI++;
				outLen += strlen(selWord);
			}
		}

		// When X button is pressed, 
		// remove the last word added to outputArr
		if (bState.x) {
			// Make sure there is *actually* something to remove
			if (outI > 0) {
				outLen -= strlen(outputArr[outI]);
				outI--;
			}
		}

		// When circle button is pressed,
		// capitalize the next word added to outputArr
		if (bState.cir) {
			capNext = 1;
		}

		// -------------------- Output Handling --------------------

		// Print the outputArr string to the screen
		printOutput(outputArr, outI);

		refresh();
	}

	endwin();

    return 0;
}

// reads words from the file
// into wl and trims the whitespace off of the end of each word
// Students are REQUIRED to use fgets() to retreive each line from the file. 
int readWords(char* w1[MAXWORDS], char* filename) {
	int i = 0;
	
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

// modifies s to trim white space off the right side
void trimws(char* s) {
	int i;

	for (i = 0; i < WORDLEN; i++) {
		if (s[i] == '\n' || s[i] == ' ') {
			s[i] = '\0';
			break;
		}
	}
}

void draw_words(char* wordlist[MAXWORDS], int wordCount) {
	int row, col;
	int i;

	for (i = 0; i < wordCount; i++) {
		row = i / MAXCOLS;
		col = i % MAXCOLS;

		mvprintw(row, (col * WORDSPACING) + LEFT_PADDING, "%s", wordlist[i]);
	}

	refresh();
}

void printOutput(char* outputArr[MAXOUTPUTLEN], int arrLength) {
	const int row = MAXWORDS / MAXCOLS;

	char* outputStr = (char*)malloc(sizeof(char)* (MAXOUTPUTLEN + 1));
	
	// Build output string
	for (int i = 0; i < arrLength; i++) {
		strcat(outputStr, outputArr[i]);	// Add word
	}

	// To prevent leftover data from previous longer outputs,
	// we use memset to change everything to blanks after the last word.
	int outputStrLen = strlen(outputStr);
	memset(outputStr, ' ', MAXOUTPUTLEN - outputStrLen);
	outputStr[MAXOUTPUTLEN] = '\0';	// Null terminate

	// Output string to screen
	mvprintw(row, 0, "Output: %s", outputStr);
}

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);    
	refresh();
}
