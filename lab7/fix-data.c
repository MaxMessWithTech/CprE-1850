#include <stdio.h>

int main(int argc, char* argv[]) {
	
    const int LINE_COUNT = 200;

    double x_inst, y_inst, z_inst = 0;
    double avg_x, avg_y, avg_z = 0;
	double max_x, max_y, max_z = 0; 
	double min_x, min_y, min_z = 0;

    printf("x,y,z,\n");

    for (int i = 0; i < LINE_COUNT; i++) {
        scanf(
            "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", 
            &x_inst, &y_inst, &z_inst, 
            &avg_x, &avg_y, &avg_y, 
            &max_x, &min_x, 
            &max_y, &min_y, 
            &max_z, &min_z
        );


		printf("%6.4lf,%6.4lf,%6.4lf,\n", x_inst, y_inst, z_inst);
    }
    return 0;

}