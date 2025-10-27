/*-----------------------------------------------------------------------------
-					          CPRE 185 Lab 02
-	Name: Maxwell (Max) Miller
- 	Section: 1670-03 & 1670-CN
-	NetID: mmill06
-	Date: 9-22-2025
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-	                            Includes
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>


/*-----------------------------------------------------------------------------
-	                            Defines
-----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
-	                            Prototypes
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
-							 Implementation
-----------------------------------------------------------------------------*/
double circle_area(double cir) {
    static double PI = 3.1415;

    // Radius = circumference / PI
    double r = cir / PI;

    // Area = PI * r^2
    return PI * r * r;
}

double ft_to_m(double ft) {
    // Conversion factor of meter per ft
    static double FT_TO_METERS = 0.3048;

    return ft * FT_TO_METERS;
}

double f_to_c(double f) {
    // c = (F - 32) / 1.8)
    return (f - 32.0) / 1.8;
}

int main()
{

    /* Put your code after this line */
    int a = 6427 + 1725;
    int b = (6971 * 3925) - 95;
    double c = 79 + 12 / 5;
    double d = 3640.0 / 107.9;
    int e = (22 / 3) * 3;
    int f = 22 / (3 * 3);
    double g = 22 / (3 * 3);
    double h = 22 / 3* 3;
    double i = (22.0 / 3) * 3.0;
    int j = 22.0 / (3 * 3.0);
    double k = 22.0 / 3.0 * 3.0;

    
    printf("6427 + 1725 = %d\n",        a);
    printf("(6971 * 3925) - 95 = %d\n", b);
    printf("79 + 12 / 5 = %.2f\n",     c);
    printf("3640.0 / 107.9 = %.2f\n",   d);
    printf("(22 / 3) * 3 = %d\n",       e);
    printf("22 / (3 * 3) = %d\n",       f);
    printf("22 / (3 * 3) = %.2f\n",     g);
    printf("22 / 3* 3 = %.2f\n",        h);
    printf("(22.0 / 3) * 3.0 = %.2f\n", i);
    printf("22.0 / (3 * 3.0) = %d\n",   j);
    printf("22.0 / 3.0 * 3.0 = %.2f\n", k);
    
    double circle_c = 23.567;
    double area = circle_area(circle_c);

    double ft = 14.0;
    double m = ft_to_m(ft);

    double degF = 76;
    double degC = f_to_c(degF);

    printf("Circle of circumference %.2f = circle of area %.2f\n", circle_c, area);
    printf("%.0f ft = %.2f m\n", ft, m);
    printf("%.0f F = %.2f c\n", degF, degC);


    return 0;

}

