/* 185 Lab 3 */

#include <stdio.h>
#include <math.h>


const int MILLIS_IN_MIN = 60000;
const int MILLIS_IN_SEC = 1000;

/* Put your function prototypes here */

double mag(double ax, double ay, double az);


int minutes(int t);
int seconds(int t);
int millis(int t);


int main(void) {
	/* DO NOT MODIFY THESE VARIABLE DECLARATIONS */
	int t;
	double  ax, ay, az;     


	/* This while loop makes your code repeat. Don't get rid of it. */
	while (1) {
		scanf("%d,%lf,%lf,%lf", &t, &ax, &ay, &az); 

		/* CODE SECTION 0 */

		// printf("Echoing output: %d, %lf, %lf, %lf\n", t, ax, ay, az);
		// printf("%8.3lf, %7.4lf, %7.4lf, %7.4lf\n", t / MILLIS_IN_SEC, ax, ay, az);

		/* CODE SECTION 1 */
		// printf("At %d ms, the acceleration's magnitude was: %lf\n", t, mag(ax, ay, az));
			
		/* CODE SECTION 2 */
		printf("At %d minutes, %d seconds, and %d milliseconds it was: %lf\n", minutes(t), seconds(t), millis(t), mag(ax,ay,az));

	}

return 0;
}

/* Put your functions here */
double mag(double ax, double ay, double az) {

	// returns the vector acceleration magnitude
	// ie. sqrt(a^2 + b^2 + c^2)
	return sqrt( ( ax * ax ) + ( ay * ay ) + ( az * az ) );
}


int minutes(int t) {
	return t / MILLIS_IN_MIN;
}


int seconds(int t) {
	return t % MILLIS_IN_MIN / MILLIS_IN_SEC;
}


int millis(int t) {
	return t % MILLIS_IN_MIN % MILLIS_IN_SEC;
}



