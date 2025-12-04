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

#define JOYSTICK_THRESHOLD 100
#define DEBOUNCE_TIME 200

typedef struct {
    int tri;	// Triangle
    int cir;	// Circle
    int x;		// Cross
    int sqr;	// Square

	int js1;	// Joy Stick 1
	int js2;	// Joy Stick 2

	int opt;	// Options
	int share;	// Share

	int tg1;	// Trigger 1
	int tg2;	// Trigger 2
	int bp1;	// Bumper 1
	int bp2;	// Bumper 2
} ButtonState;

typedef struct {
    int x;
    int y;
    int button;
} JoystickState;


/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/
int readWords(char* wl[MAXWORDS], char* filename); 
void trimws(char* s);

void draw_words(char* wordlist[MAXWORDS], int wordCount);
void printOutput(char* outputArr[], int length);
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
	int outI = -1;		// Output index in outputArr[]
	int outLen = 0;		// Current length of outputArr string

	// Current selection index
	int sel = 0;
	int newSel = 0;
	int selR = 0, selC = 0;		// Selection row and column helper variables

	char* selWord;

	// Capitalize next word flag
	int capNext = 0;

	// Variables for reading ds4rd.exe input
	int t = 0;
	int last_t = 0;
	ButtonState bState;
	JoystickState jState1;
	JoystickState jState2;

	initscr();
	refresh();

	draw_words(wordlist, wordCount);

	int numRows = wordCount / MAXCOLS;
	if (wordCount % MAXCOLS != 0) {
		numRows += 1;
	}

	draw_character(0, LEFT_PADDING - 1, '>');

	// Main loop
	while (1) {
		// Update selection index
		newSel = -1;
		selR = sel / MAXCOLS;
		selC = sel % MAXCOLS;


		// -------------------- Read ds4rd.exe Input --------------------

		// ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -b -j -bt
		// time, b1, b2, b3, b4, jx1, jy1, jx2, jy2, j-button
		scanf(
			"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			&t, 
			&bState.tri, &bState.cir, &bState.x, &bState.sqr, 
			&bState.js1, &bState.js2, &bState.opt, &bState.share, 
			&bState.tg1, &bState.tg2, &bState.bp1, &bState.bp2,
			&jState1.x, &jState1.y, &jState2.x, &jState2.y
		);

		if (t - last_t < DEBOUNCE_TIME) {
			continue;
		}


		// -------------------- Joystick Handling --------------------

		// Allow user to move selection cursor with joystick in the x direction
		// and y direction.  Wrap around when reaching the edges of the screen.
		// If the joystick is moved right, move selection right 
		if (jState1.x > JOYSTICK_THRESHOLD && sel < wordCount - 1) {
			newSel = sel + 1;
		} 
		// If the joystick is moved left, move selection left
		else if (jState1.x < -JOYSTICK_THRESHOLD && sel > 0) {
			newSel = sel - 1;
		}

		// If joystick 2 is moved right, move selection right
		else if (jState2.x > JOYSTICK_THRESHOLD && sel < wordCount - 1) {
			newSel = sel + 1;
		} 
		// If joystick 2 is moved left, move selection left
		else if (jState2.x < -JOYSTICK_THRESHOLD && sel > 0) {
			newSel = sel - 1;
		}

		// Otherwise if joystick 1 or 2 is moved down, clear output
		else if (jState1.y > JOYSTICK_THRESHOLD || jState2.y > JOYSTICK_THRESHOLD) {
			last_t = t;
			outI = -1;
			outLen = 0;
		}

		if (newSel != -1) {
			last_t = t;
			// Erase previous selection cursor
			draw_character(selC * WORDSPACING, LEFT_PADDING + selR - 1, ' ');

			sel = newSel;
			selR = sel / MAXCOLS;
			selC = sel % MAXCOLS;
			// Display the selection cursor
			draw_character(selC * WORDSPACING, LEFT_PADDING + selR - 1, '>');
		}


		// -------------------- Button Handling --------------------

		// selWord points to the currently selected word
		selWord = wordlist[sel];

		// When triangle button is pressed, 
		// add a space along with the currently selected word to the outputArr
		if (bState.tri) {
			last_t = t;
			// Make sure we don't overflow outputArr, 
			// or exceed MAXOUTPUTLEN when the output string is built in printOutput()
			// +1 for space
			if (outLen + strlen(selWord) + 1 < MAXOUTPUTLEN) {
				char* tempWord = (char*)malloc(sizeof(char) * MAXOUTPUTLEN);
				
				strcpy(tempWord, selWord);	// Copy selected word to tempWord
				
				if (capNext) {
					
					// Capitalize the first letter of selWord
					tempWord[0] = toupper(tempWord[0]);
					capNext = 0;	// Reset capitalize flag
				}

				char *new_str = (char *)malloc(sizeof(char) * (WORDLEN + 2)); 
				
				new_str[0] = ' '; // Place the space at the beginning
				strcpy(new_str + 1, tempWord); // Copy the original string after the space

				outputArr[++outI] = new_str;

				// Increment output length
				outLen += strlen(tempWord); // +1 for space
			}
		}

		// When square button is pressed, 
		// add the currently selected word to the outputArr string
		if (bState.sqr) {
			last_t = t;
			// Make sure we don't overflow outputArr, 
			// or exceed MAXOUTPUTLEN when the output string is built in printOutput()
			if (outLen + strlen(selWord) < MAXOUTPUTLEN) {
				char* tempWord = (char*)malloc(sizeof(char) * MAXOUTPUTLEN);

				strcpy(tempWord, selWord);	// Copy selected word to tempWord

				if (capNext) {
					// Capitalize the first letter of selWord
					tempWord[0] = toupper(selWord[0]);
					capNext = 0;	// Reset capitalize flag
				}
				
				outputArr[++outI] = tempWord;

				// Increment output length
				outLen += strlen(tempWord);
			}
		}

		// When X button is pressed, 
		// remove the last word added to outputArr
		if (bState.x) {
			last_t = t;
			// Make sure there is *actually* something to remove
			if (outI >= 0) {
				outLen -= strlen(outputArr[outI]);
				outI--;
			}
		}

		// When circle button is pressed,
		// capitalize the next word added to outputArr
		if (bState.cir) {
			last_t = t;
			capNext = 1;
		}

		// -------------------- Output Handling --------------------

		// Print the outputArr string to the screen
		printOutput(outputArr, outI + 1);

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

void printOutput(char* outputArr[], int arrLength) {
	// char outputStr[MAXOUTPUTLEN + 1] = "";
	char *outputStr = (char *)malloc(MAXOUTPUTLEN + 1);

	memset(outputStr, ' ', MAXOUTPUTLEN); // Fill the allocated memory with spaces
    outputStr[MAXOUTPUTLEN] = '\0';        // Null-terminate the string
	
	int k = 0;
	
	// Build output string
	for (int i = 0; i < arrLength; i++) {
		for (int j = 0; j < strlen(outputArr[i]) - 1; j++) {
			outputStr[k++] = outputArr[i][j];
			
		}
	}

	// To prevent leftover data from previous longer outputs,
	// we use memset to change everything to blanks after the last word.
	// int outputStrLen = strlen(outputStr);
	// memset(outputStr, ' ', MAXOUTPUTLEN - outputStrLen);
	// outputStr[MAXOUTPUTLEN - 1] = '\0';	// Null terminate

	// Output string to screen
	mvprintw(MAXWORDS / MAXCOLS, 0, "Output: %s", outputStr);
	refresh();

	free(outputStr);
}

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);    
	refresh();
}
