// 185 Lab 7
#include <stdio.h>
#include <float.h>  // for DBL_MAX and DBL_MIN


#define MAXPOINTS 10000

// compute the average of the first num_items of buffer
double avg(double buffer[], int num_items);

//update the max and min of the first num_items of array
void maxmin(double array[], int num_items, double* max, double* min);

//shift length-1 elements of the buffer to the left and put the 
//new_item on the right.
void updatebuffer(double buffer[], int length, double new_item);


/*
Args (in order): 
*/
int main(int argc, char* argv[]) {
	
	/* DO NOT CHANGE THIS PART OF THE CODE */

	double x[MAXPOINTS], y[MAXPOINTS], z[MAXPOINTS];

	// Length of average
	int lengthofavg = 0;
	if (argc>1) {
		sscanf(argv[1], "%d", &lengthofavg );
		printf("You entered a buffer length of %d\n", lengthofavg);
	}
	else {
		printf("Enter a length on the command line\n");
		return -1;
	}
	if (lengthofavg <1 || lengthofavg >MAXPOINTS) {
		printf("Invalid length\n");
		return -1;
	}


	
	/* PUT YOUR CODE HERE */
	printf("x,y,z,x avg,y avg,z avg,x max,x min,y max,y min,z max,z min");

	double x_inst, y_inst, z_inst = 0;
	int b_tri, b_cir, b_x, b_sqr = 0;

	double avg_x, avg_y, avg_z = 0;
	double max_x, max_y, max_z = DBL_MIN; 
	double min_x, min_y, min_z = DBL_MAX;

	int i = 0;
	
	do {
		scanf("%lf, %lf, %lf, %d, %d, %d, %d", &x_inst, &y_inst, &z_inst, &b_tri, &b_cir, &b_x, &b_sqr);
		
		updatebuffer(x, lengthofavg, x_inst);
		updatebuffer(y, lengthofavg, y_inst);
		updatebuffer(z, lengthofavg, z_inst);
		
		if (i < lengthofavg) {
			i++;
			continue;
		}
		avg_x = avg(x, lengthofavg);
		avg_y = avg(y, lengthofavg);
		avg_z = avg(z, lengthofavg);

		maxmin(x, lengthofavg, &max_x, &min_x);
		maxmin(y, lengthofavg, &max_y, &min_y);
		maxmin(z, lengthofavg, &max_z, &min_z);

		printf("%6.4lf,%6.4lf,%6.4lf,", x_inst, y_inst, z_inst);
		printf("%6.4lf,%6.4lf,%6.4lf,", avg_x, avg_y, avg_y);
		
		printf("%6.4lf,%6.4lf,", max_x, min_x);
		printf("%6.4lf,%6.4lf,", max_y, min_y);
		printf("%6.4lf,%6.4lf\n", max_z, min_z);

		fflush(stdout);


	}
	while (!b_sqr);


}


double avg(double buffer[], int num_items) {

	double t = 0;
	for (int i = 0; i < num_items; i++) {
		t += buffer[i];
	}

	return t / num_items;
}

void maxmin(double array[], int num_items, double* max, double* min) {
	double max_temp, min_temp = array[0];

	for (int i = 1; i < num_items; i++) {
		if (array[i] > max_temp) {
			max_temp = array[i];
		}

		else if (array[i] < *min) {
			min_temp = array[i];
		}
	}

	*max = max_temp;
	*min = min_temp;
}

void updatebuffer(double buffer[], int length, double new_item) {
	for (int i = 0; i < length - 2; i++) {
		buffer[i] = buffer[i + 1];
	}
	buffer[length - 1] = new_item;
}