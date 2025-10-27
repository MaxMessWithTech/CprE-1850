/* 185 Lab 3-2 */

#include <stdio.h>
#include <math.h>


/* Put your function prototypes here */
int numButtons(int a, int b, int c, int d);


int main(void) {
	/* DO NOT MODIFY THESE VARIABLE DECLARATIONS */
	int t;
	int a,b,c,d;


	/* This while loop makes your code repeat. Don't get rid of it. */
	while (1) {
		scanf("%d,%d,%d,%d", &a, &b, &c, &d); 
		printf("Buttons pressed: %d\n", numButtons(a,b,c,d));

		fflush(stdout);
	}

return 0;
}

/* Put your functions here */
int numButtons(int a, int b, int c, int d) {
	return (a + b + c + d);
}


