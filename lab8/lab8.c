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


// 2D character array which the maze is mapped into
char MAZE[NUMROWS][NUMCOLS];


// POST: Generates a random maze structure into MAZE[][]
//You will want to use the rand() function and maybe use the output %100.  
//You will have to use the argument to the command line to determine how 
//difficult the maze is (how many maze characters are on the screen).
void generate_maze(int difficulty);

// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen 
void draw_maze(void);

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use);

// PRE: -1.0 < x_mag < 1.0
// POST: Returns tilt magnitude scaled to -1.0 -> 1.0
// You may want to reuse the roll function written in previous labs.  
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
		draw_character(avatar_x,avatar_y,MAZE[avatar_y][avatar_x]);
		
		/*
		0
		1
		2 - 1 > 0 
		*/
		
		roll = calc_roll(x);
		if (roll > 0.4 && avatar_x - 1 > 0 && MAZE[avatar_y][avatar_x - 1] != WALL) {
			avatar_x--;
		}
		else if (roll < -0.4 && avatar_x + 1 < NUMCOLS && MAZE[avatar_y][avatar_x + 1] != WALL) {
			avatar_x++;
		}

		// Is it time to move?  if so, then move avatar
		if (t / MOVE_DELAY > last_move_time) {
			last_move_time = t / MOVE_DELAY;

			
			if (avatar_y + 1 >= NUMROWS) {	break;		}
			if (MAZE[avatar_y + 1][avatar_x] != WALL) {
				avatar_y++;
			}
		}
		draw_character(avatar_x,avatar_y,AVATAR);

		
	} while(1); // Change this to end game at right time 

	// Print the win message
	endwin();
	
	printf("YOU WIN!\n");
	return 0;
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