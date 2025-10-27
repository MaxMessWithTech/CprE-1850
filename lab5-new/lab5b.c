/*-----------------------------------------------------------------------------
-					    CprE 1850 Lab 05 - Part 2
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>


const double FALL_TOL = 0.2;
const double MOTION_TOL = 0.05;
const double JERK_TOL = 0.4;
const double FALL_ACCEL = 0.0;
const double STILL_ACCEL = 1.0;
const double G = 9.8; // m/s^2


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
	STOPPED
};


/*-----------------------------------------------------------------------------
								Prototypes
-----------------------------------------------------------------------------*/
double mag(Vector3D accel);
int close_to(double tolerance, double point, double value);

Vector3D getInput(void);
double fallDist(double t);


/*-----------------------------------------------------------------------------
							  Implementation
-----------------------------------------------------------------------------*/
int main(void) {
	int ts;
	Vector3D tempAccel;
	Vector3D accel;

	double prev_mag;
	double curr_mag;

	double v = 0.0;
	double x = 0.0;
	double delta_sec;

	enum FallState fs = UNKNOWN;

	fflush(stdout);
	printf("Maxwell Miller\n");
	printf("mmill06\n");

	// Won't release until it has gotten the first input
	accel = getInput();

	printf("Ok, I'm now receiving data.\n");
	printf("I'm Waiting");
	fflush(stdout);

	int i = 0;

	while (fs == UNKNOWN || fs == STILL) {
		// Get values from ds4rd.exe
		tempAccel = getInput();
		prev_mag = mag(accel);
		curr_mag = mag(tempAccel);

		// Is falling if there's a significant change in 
		// acceleration magnitude that has a negative trend
		if (close_to(FALL_TOL, curr_mag, FALL_ACCEL)) {
			fs = FALLING;
			break;
		}
		else if (fs == UNKNOWN && prev_mag - curr_mag > MOTION_TOL) {
			delta_sec = (double)(tempAccel.t - accel.t) / 1000.0;
			v += G * (1.0 - mag(tempAccel)) * delta_sec;
			x += v * delta_sec;
		}
		else if (fs != UNKNOWN && prev_mag - curr_mag > MOTION_TOL) {
			fs = UNKNOWN;
			ts = tempAccel.t;
			x = 0.0;
			v = 0.0;
		}
		else if (fs == UNKNOWN && prev_mag < curr_mag) {
			fs = STILL;
		}

		if (i % 5 == 0) {
			printf(".");
			fflush(stdout);
		}

		accel = tempAccel;
		i++;
	}

	printf("\n\n\tHelp me! I'm Falling");
	
	// While falling
	while (1) {
		tempAccel = getInput();

		if (close_to(MOTION_TOL, mag(accel), STILL_ACCEL) || mag(tempAccel) - mag(accel) > JERK_TOL) {
			fs = STOPPED;
			break;
		}
		
		delta_sec = (double)(tempAccel.t - accel.t) / 1000.0;
		v += G * (1.0 - mag(tempAccel)) * delta_sec;
		x += v * delta_sec;

		// Only print every 4th time value to reduce spam
		if (i % 4 == 0) {
			printf("!");
		}

		accel = tempAccel;
		i++;
	}

	// Final Time in ms
	int tf = accel.t;

	// Delta Time in seconds
	delta_sec = (double)(tf - ts) / 1000.0;

	double dist = fallDist(delta_sec);
	int diff = (dist - x) / dist * 100;


	printf("\n");
	printf("\n\t\tOuch! I fell %4.3lf meters in %4.3lf seconds.\n", fallDist(delta_sec), delta_sec);

	printf("\t\tCompensating for air resistance, the fall was %4.3lf meters.\n", x);
	printf("\t\tThis is %d%% less than computed before.\n", diff);


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
	// Derived from the equation of motion for constant acceleration
	// d = v_i * t + 1/2 * a * t^2  -->  d = 1/2 * G * t^2
	return 0.5 * G * t * t; // meters
}