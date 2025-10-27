/*-----------------------------------------------------------------------------
-					         CprE 1850 Lab 05
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-	                            Includes
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>


/*-----------------------------------------------------------------------------
-	                            Defines
-----------------------------------------------------------------------------*/
typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    double b1;
    double b2;
    double b3;
    double b4;
} ButtonState;

enum FallState {
    STILL,
    FALLING_START,
    TERMINAL,
    DECELERATING,
    STOPPED
};


/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/
double mag(Vector3D accel);
int close_to(double tolerance, double point, double value);
int in_motion(Vector3D accel);


/*-----------------------------------------------------------------------------
-							  Implementation
-----------------------------------------------------------------------------*/
int main(void) {
    int t;
    Vector3D accel;
    Vector3D g_accel;
    ButtonState bs;


	while (1) {
        // Get values from ds4rd.exe
		scanf("%d, %lf, %lf, %lf, %lf, %lf, %lf, %d, %d, %d, %d", &t, 
            &accel.x, &accel.y, &accel.z, &g_accel.x, &g_accel.y, &g_accel.z,
            &bs.b1, &bs.b2, &bs.b3, &bs.b4);

		/* printf for observing values scanned in from ds4rd.exe, be sure to comment or remove in final program */
		// printf("Echoing output: %d, %lf, %lf, %lf, %lf, %lf, %lf, %d, %d, %d, %d \n", t, ax, ay, az, gx, gy, gz, b1, b2, b3, b4);
		// printf("Echoing output: %d, %lf, %lf, %lf\n", t, gx, gy, gz);
        
        printf("|<%0.6lf, %0.6lf, %0.6lf>| = %0.6lf\n", accel.x, accel.y, accel.z, mag(accel));

		/* It would be wise (mainly save time) if you copy your code to calculate the magnitude from last week
		 (lab 3).  You will also need to copy your prototypes and functions to the appropriate sections
		 in this program. */

		// printf("\tAt %d ms, the acceleration's magnitude was: %f\n", t, mag(ax, ay, az));

		int motion = in_motion(accel);


		// If Triangle button is pressed, end the program
		if (bs.b1) {
			break;
		}

	}

	return 0;
}


/**
 * Finds the magnitude of a 3D vector
 * 
 * @param vec Vector3D
 * @returns The vector magnitude
 */
double mag(Vector3D vec) {

	// returns the vector acceleration magnitude
	// ie. sqrt(a^2 + b^2 + c^2)
	return sqrt( ( vec.x * vec.x ) + ( vec.y * vec.y ) + ( vec.z * vec.z ) );
}


/**
 * Finds if two doubles are close to each other within a given tolerance
 * 
 * @param tolerance Tolerance for the difference between val1 and val2.
 * @param val1 Double 1
 * @param val2 Double 2
 * @return 0 if not within tolerance, 1 if it is within tolerance.
 */
int close_to(double tolerance, double val1, double val2) {
	return fabs(val1 - val2) <= tolerance;
}


/**
 * Finds if the controller is still or in motion
 * 
 * @param accel Vector3D acceleration
 * @returns The vector magnitude
 */
int in_motion(Vector3D accel) {
	const double MOTION_TOL = 0.05;

	double aMag = mag(accel);

	return !close_to(MOTION_TOL, 0.0, aMag);
}