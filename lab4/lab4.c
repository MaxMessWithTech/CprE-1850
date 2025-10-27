/*-----------------------------------------------------------------------------
-					         SE/CprE 185 Lab 04
-             Developed for 185-Rursch by T.Tran and K.Wang
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-	                            Includes
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>


/*-----------------------------------------------------------------------------
-	                            Defines
-----------------------------------------------------------------------------*/

enum Direction {
	TOP,
	BOTTOM,
	RIGHT,
	LEFT,
	FRONT,
	BACK,
	NONE
};


/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/
double mag(double ax, double ay, double az);
int close_to(double tolerance, double point, double value);
int in_motion(double ax, double ay, double az);

int is_top(double gx, double gy, double gz);
int is_bot(double gx, double gy, double gz);
int is_right(double gx, double gy, double gz);
int is_left(double gx, double gy, double gz);
int is_front(double gx, double gy, double gz);
int is_back(double gx, double gy, double gz);


/*-----------------------------------------------------------------------------
-							  Implementation
-----------------------------------------------------------------------------*/
int main(void) {
	int t, b1, b2, b3, b4;
	double ax, ay, az, gx, gy, gz;

	enum Direction lastDirection = NONE;


	while (1) {
		scanf("%d, %lf, %lf, %lf, %lf, %lf, %lf, %d, %d, %d, %d", &t, &ax, &ay, &az, &gx, &gy, &gz, &b1, &b2, &b3, &b4 );

		/* printf for observing values scanned in from ds4rd.exe, be sure to comment or remove in final program */
		// printf("Echoing output: %d, %lf, %lf, %lf, %lf, %lf, %lf, %d, %d, %d, %d \n", t, ax, ay, az, gx, gy, gz, b1, b2, b3, b4);
		// printf("Echoing output: %d, %lf, %lf, %lf\n", t, gx, gy, gz);

		/* It would be wise (mainly save time) if you copy your code to calculate the magnitude from last week
		 (lab 3).  You will also need to copy your prototypes and functions to the appropriate sections
		 in this program. */

		// printf("\tAt %d ms, the acceleration's magnitude was: %f\n", t, mag(ax, ay, az));

		int motion = in_motion(ax, ay, az);

		if (in_motion(ax, ay, az)) {
			// DO NOTHING
		}
		else if (is_top(gx, gy, gz) && lastDirection != TOP) {
			lastDirection = TOP;
			printf("TOP\n");
		}
		else if (is_bot(gx, gy, gz) && lastDirection != BOTTOM) {
			lastDirection = BOTTOM;
			printf("BOTTOM\n");
		}
		else if (is_right(gx, gy, gz) && lastDirection != RIGHT) {
			lastDirection = RIGHT;
			printf("RIGHT\n");
		}
		else if (is_left(gx, gy, gz) && lastDirection != LEFT) {
			lastDirection = LEFT;
			printf("LEFT\n");
		}
		else if (is_front(gx, gy, gz) && lastDirection != FRONT) {
			lastDirection = FRONT;
			printf("FRONT\n");
		}
		else if (is_back(gx, gy, gz) && lastDirection != BACK) {
			lastDirection = BACK;
			printf("BACK\n");
		}


		// If Triangle button is pressed, end the program
		if (b1) {
			break;
		}

	}

	return 0;
}


double mag(double ax, double ay, double az) {

	// returns the vector acceleration magnitude
	// ie. sqrt(a^2 + b^2 + c^2)
	return sqrt( ( ax * ax ) + ( ay * ay ) + ( az * az ) );
}


/* Put your functions here */
int close_to(double tolerance, double point, double value) {
	if (0) {
		printf(
			"(fabs(%lf - %lf) <= %lf) == %lf <= %lf == %d \t", 
			point, value, tolerance, 
			fabs(point - value), tolerance, 
			fabs(point - value) <= tolerance
		);
	}

	return fabs(point - value) <= tolerance;
}

int in_motion(double ax, double ay, double az) {
	const double MOTION_TOL = 0.05;

	double aMag = mag(ax, ay, az);

	return !close_to(MOTION_TOL, 0.0, aMag);
}


int is_top(double gx, double gy, double gz) {
	const double X_TOL = 0.4;
	const double Y_TOL = 0.25;  // DERTMINANING TOLERENCE
	const double Z_TOL = 0.4;

	const double TARGET_GX = 0.035;
	const double TARGET_GY = 0.98;
	const double TARGET_GZ = 0.199;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;
}

int is_bot(double gx, double gy, double gz) {
	const double X_TOL = 0.4;
	const double Y_TOL = 0.25;  // DERTMINANING TOLERENCE
	const double Z_TOL = 0.4;

	const double TARGET_GX = 0.0;
	const double TARGET_GY = -1.0;
	const double TARGET_GZ = 0.0;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;

}

int is_right(double gx, double gy, double gz) {
	const double X_TOL = 0.25;  // DERTMINANING TOLERENCE
	const double Y_TOL = 0.4;
	const double Z_TOL = 0.4;

	const double TARGET_GX = -1.0;
	const double TARGET_GY = 0.0;
	const double TARGET_GZ = 0.0;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;
}

int is_left(double gx, double gy, double gz) {
	const double X_TOL = 0.25;  // DERTMINANING TOLERENCE
	const double Y_TOL = 0.4;
	const double Z_TOL = 0.4;

	const double TARGET_GX = 1.0;
	const double TARGET_GY = 0.0;
	const double TARGET_GZ = 0.0;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;
}

int is_front(double gx, double gy, double gz) {
	const double X_TOL = 0.4;
	const double Y_TOL = 0.4;
	const double Z_TOL = 0.25;  // DERTMINANING TOLERENCE

	const double TARGET_GX = 0.0;
	const double TARGET_GY = 0.0;
	const double TARGET_GZ = -1.0;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;
}

int is_back(double gx, double gy, double gz) {
	const double X_TOL = 0.4;
	const double Y_TOL = 0.4;
	const double Z_TOL = 0.25; // DERTMINANING TOLERENCE

	const double TARGET_GX = 0.0;
	const double TARGET_GY = 0.0;
	const double TARGET_GZ = 1.0;

	int a = close_to(X_TOL, TARGET_GX, gx);
	int b = close_to(Y_TOL, TARGET_GY, gy);
	int c = close_to(Z_TOL, TARGET_GZ, gz);

	return a && b && c;
}