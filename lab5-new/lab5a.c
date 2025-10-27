/*-----------------------------------------------------------------------------
-					         CprE 1850 Lab 05
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>


typedef struct {
	double x;
	double y;
	double z;
	int t;
} Vector3D;

enum FallState {
	UNKNOWN,
	STILL,
	FALLING,
	DECELERATING,
	STOPPED
};


/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/
double mag(Vector3D accel);
int close_to(double tolerance, double point, double value);
int in_motion(Vector3D accel);

Vector3D getInput(void);
double fallDist(double t);


/*-----------------------------------------------------------------------------
-							  Implementation
-----------------------------------------------------------------------------*/
int main(void) {
	int ts;
	int tf;
	Vector3D accel;
	enum FallState fs = UNKNOWN;

	fflush(stdout);
	printf("Maxwell Miller\n");
	printf("mmill06\n");

	accel = getInput();

	printf("Ok, I'm now receiving data.\n");
	printf("I'm Waiting");
	fflush(stdout);


	while (fs != STOPPED) {
		// Get values from ds4rd.exe
		accel = getInput();
		
		if (in_motion(accel) && fs != UNKNOWN) {
			double a_mag = mag(accel);

			// If the acceleration is approaching zero, then it just started falling
			if (close_to(0.2, a_mag, 0.0) && fs != FALLING) {
				fs = FALLING;
				
				ts = accel.t;
				printf("\n\n\tHelp me! I'm Falling");
			}
			// If it is falling 
			else if (fs == FALLING) {
				// If we're in motion but no longer approaching zero, 
				// then we have hit the ground
				if (!close_to(0.2, a_mag, 0.0)) {
					// printf("\nStopping with acceleration of %lf\n", a_mag);
					fs = STOPPED;
					break;
				}

				if (accel.t % 10 == 0) {
					printf("!");
				}
			}
			else {
				fs = UNKNOWN;
			}
		}
		
		// If it is not in motion but it was falling, then say the fall has ended, breaking the while loop
		else if (!in_motion(accel) && fs == FALLING) {
			printf("\nStopping 2\n");
			fs = STOPPED;
		}
		
		// If it is not in motion, just keep waiting for it to be 
		else if (!in_motion(accel)) {
			fs = STILL;

			if (accel.t % 10 == 0) {
				printf(".");
			}
		}
		
		// If it makes it down here then it must be IN MOTION but hasn't fallen yet. 
		// Therefore we must keep waiting while it is in the UNKNOWN state
		else {
			if (accel.t % 100 == 0) {
				printf(".");
			}
		}
		fflush(stdout);
	}

	tf = accel.t;

	double delta_sec = (double)(tf - ts) / 1000.0;
	printf("\n");
	printf("\n\t\tOuch! I fell %4.3lf meters in %4.3lf seconds.\n", fallDist(delta_sec), delta_sec);

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

	return !close_to(MOTION_TOL, 1.0, aMag);
}


/**
 * Gets input from the user
 * @returns A Vector3D struct with the input values
 */
Vector3D getInput(void) {
	Vector3D v;

	scanf("%d, %lf, %lf, %lf", &v.t, &v.x, &v.y, &v.z);

	return v;
}

/**
 * Calculates the distance fallen in a given time
 * 
 * @param t Time in seconds
 * @returns Distance fallen in meters
 */
double fallDist(double t) {
	const double g = 9.8; // m/s^2
	// Derived from the equation of motion for constant acceleration
	// d = v_i * t + 1/2 * a * t^2  -->  d = 1/2 * g * t^2
	return 0.5 * g * t * t; // meters
}
