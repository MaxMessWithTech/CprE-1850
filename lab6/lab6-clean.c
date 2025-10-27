// 185 lab6.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589

// Functional Prototypes
int read_line(int* time, double* g_x, double* g_y, double* g_z, int* Button_T, int* Button_C, int* Button_X, int* Button_S);
double roll(double x_mag);
double pitch(double y_mag);
int scaleRadsForScreen(double rad);
void print_chars(int num, char use);
void graph_line(int number);

// Enum for mode selection with the controller button
enum Mode {
	ROLL_MODE,
	PITCH_MODE
};

int main()
{
	double x, y, z;                             // magnitude values of x, y, and z
	int b_Triangle, b_X, b_Square, b_Circle;    // variables to hold the button statuses
	double roll_rad, pitch_rad;                 // value of the roll measured in radians
	int scaled_value;                           // value of the roll adjusted to fit screen display
	
	int time;
	enum Mode mode = ROLL_MODE;					// Default to roll mode
	int val;

	do
	{
		// Get line of input
		read_line(&time, &x, &y, &z, &b_Triangle, &b_Circle, &b_X, &b_Square);

		// calculate roll and pitch.  Use the buttons to set the condition for roll and pitch
		roll_rad = roll(x);
		pitch_rad = pitch(y);

		// switch between roll and pitch(up vs. down button)
		if (b_Triangle) {
			if (mode == ROLL_MODE) {
				mode = PITCH_MODE;
			}
			else {
				mode = ROLL_MODE;
			}
		}

		// Scale your output value
		if (mode == ROLL_MODE) {
			val = scaleRadsForScreen(roll_rad);
		}
		else if (mode == PITCH_MODE) {		// Written this way for clarity
			val = scaleRadsForScreen(pitch_rad);
		}

		// Output your graph line
		graph_line(val);

		fflush(stdout);
	} while (!b_Square); // Stops when the square button is pressed
	
	return 0;
}


int read_line(int* time, double* g_x, double* g_y, double* g_z, int* Button_T, int* Button_C, int* Button_X, int* Button_S) {
	scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", time, g_x, g_y, g_z, Button_T, Button_C, Button_X, Button_S);

	return (*Button_S);
}

double roll(double x_mag) {
	if (x_mag > 1.0) 	{	x_mag = 1.0;	}
	if (x_mag < -1.0) 	{	x_mag = -1.0;	}

	// Negative to make the roll make visual sense on the screen
	// ie Left is left, right is right
	return -asin(x_mag);
}

double pitch(double y_mag) {
	if (y_mag > 1.0) 	{	y_mag = 1.0;	}
	if (y_mag < -1.0) 	{	y_mag = -1.0;	}

	return asin(y_mag);
}

int scaleRadsForScreen(double rad) {
	// -PI/2 to PI/2 -> -1 to 1 -> -39 to 39
	return (rad * (2 / PI)) * 39;
}

void print_chars(int num, char use) {
	for (int i = 0; i < num; i++) {
		printf("%c", use);
	}
}

void graph_line(int number) {
	int side = 39 - abs(number);

	if (number < 0) {
		print_chars(side, ' ');
		print_chars(abs(number), '|');
	}

	else if (number == 0) {
		print_chars(39, ' ');
		print_chars(1, '0');
	}

	else {
		print_chars(39, ' ');
		print_chars(number, 'r');
	}

	print_chars(1, '\n');
}