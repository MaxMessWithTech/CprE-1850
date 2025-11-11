// WII-MAZE Skeleton code written by Jason Erbskorn 2007
// Edited for ncurses 2008 Tom Daniels
// Updated for Esplora 2013 TeamRursch185
// Updated for DualShock 4 2016 Rursch


// Headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>
#include <unistd.h>

// Mathematical constants
#define PI 3.14159

// Screen geometry
// Use ROWS and COLS for the screen height and width (set by system)
// MAXIMUMS
#define NUMCOLS 100
#define NUMROWS 72

// Character definitions taken from the ASCII table
#define AVATAR 'A'
#define WALL '*'
#define EMPTY_SPACE ' '

#define MOVE_DELAY 200
#define ROLL_THRESHOLD 0.4


// 2D character array which the maze is mapped into
char MAZE[NUMROWS][NUMCOLS];


int isCellOpen(int x, int y);
int canMoveLeft(int x, int y);
int canMoveRight(int x, int y);
int canMoveDown(int x, int y);
void erase_avatar(int x, int y);
void generate_maze(int difficulty);
void draw_maze(void);
void draw_character(int x, int y, char use);
float calc_roll(float x_mag);


// Main - Run with './ds4rd.exe -t -g -b' piped into STDIN
int main(int argc, char* argv[])
{
	if (argc <2) { printf("You forgot the difficulty\n"); return 1;}
	int difficulty = atoi(argv[1]); // get difficulty from first command line arg
	// setup screen    
	initscr();
	refresh();


	int t;
	double x,y,z;
	
	int avatar_x = NUMCOLS / 2;
	int avatar_y = 0;
	int last_move_time = 0;

	double roll = 0.0;

	
	// Generate and draw the maze, with initial avatar
	generate_maze(difficulty);
	draw_maze();

	draw_character(avatar_x, avatar_y, AVATAR);
	
	// Read gyroscope data to get ready for using moving averages.  


	// Event loop
	do
	{
		// Read data, update average
		scanf("%d, %lf, %lf, %lf", &t, &x, &y, &z);
		draw_character(avatar_x, avatar_y, MAZE[avatar_y][avatar_x]);

		// Is it time to move?  if so, then move avatar
		if (t / MOVE_DELAY > last_move_time) {
			last_move_time = t / MOVE_DELAY;

			roll = calc_roll(x);

			// Left Move? 
			if (roll > ROLL_THRESHOLD && canMoveLeft(avatar_x, avatar_y)) {
				avatar_x--;
			}

			// Right Move?
			else if (roll < -ROLL_THRESHOLD && canMoveRight(avatar_x, avatar_y)) {
				avatar_x++;
			}

			// Can we move down?
			if (canMoveDown(avatar_x, avatar_y)) {
				avatar_y++;
			}

			erase_avatar(avatar_x,avatar_y);
			draw_character(avatar_x,avatar_y,AVATAR);

			// Can't Move
			// BONUS #1/#2
			if (!canMoveLeft(avatar_x, avatar_y) && !canMoveRight(avatar_x, avatar_y) && !canMoveDown(avatar_x, avatar_y)) {
				printf("STUCK! GAME OVER!\n");
				return 0;
			}
		}

		
	} while(avatar_y + 1 < NUMROWS); // Have we won?

	// Print the win message
	endwin();
	
	printf("YOU WIN!\n");
	return 0;
}


// PRE: 0 < x < COLS, 0 < y < ROWS
// POST: Returns 1 if cell is open, 0 if cell is a wall or out of bounds
int isCellOpen(int x, int y) {
	if (x < 0 || x >= NUMCOLS || y < 0 || y >= NUMROWS) {
		return 0;
	}
	if (MAZE[y][x] == WALL) {
		return 0;
	}
	return 1;
}

int canMoveLeft(int x, int y) {
	return isCellOpen(x - 1, y);;
}

int canMoveRight(int x, int y) {
	return isCellOpen(x + 1, y);
}

int canMoveDown(int x, int y) {
	return isCellOpen(x, y + 1);
}

void erase_avatar(int x, int y) {
	draw_character(x, y, MAZE[y][x]);
}



// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
//THIS CODE FUNCTIONS FOR PLACING THE AVATAR AS PROVIDED.
//
//    >>>>DO NOT CHANGE THIS FUNCTION.<<<<
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);    
	refresh();
}


// POST: Generates a random maze structure into MAZE[][]
// You will want to use the rand() function and maybe use the output %100.  
// You will have to use the argument to the command line to determine how 
// difficult the maze is (how many maze characters are on the screen).
void generate_maze(int difficulty) {
	int i,j;
	int random = 0.0;

	for(i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {

			if (difficulty == 0) {
				MAZE[i][j] = EMPTY_SPACE;
				continue;
			}

			random = rand() % 100;

			if (random < difficulty) {
				MAZE[i][j] = WALL;
				continue;
			}

			MAZE[i][j] = EMPTY_SPACE;
		}
	}
}


// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen 
void draw_maze(void) {
	int i,j;

	for(i = 0; i < NUMROWS; i++) {
		for (j = 0; j < NUMCOLS; j++) {
			mvaddch(i,j,MAZE[i][j]);
		}
	}
}


// PRE: -1.0 < x_mag < 1.0
// POST: Returns tilt magnitude scaled to -1.0 -> 1.0
// You may want to reuse the roll function written in previous labs.  
float calc_roll(float x_mag) {
	if (x_mag > 1.0) 	{	return 1.0;		}
	if (x_mag < -1.0) 	{	return -1.0;	}

	return x_mag;
}