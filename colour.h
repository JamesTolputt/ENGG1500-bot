#ifndef COLOUR_H
#define COLOUR_H

typedef struct {
	double red;
	double green;
	double blue;
	double lightness;
	int hue;
	double saturation;
} colour;


//https://www.rapidtables.com/convert/color/rgb-to-hsl.html rgb to hsl conversion refrence

//Function to move the value into the range 0 < x < 1
//expects a value that has some decimal component
double testCrush(double i);

//tests the values of red, green and blue to calculate 
double test(double i, double tmp1, double tmp2);

void maxMin(double* arr, double* max, double* min, char* rgb);

//function to calculate the hue of a colour given its red green and blue values
double calculate_hue(double* a, char rgb, double max, double min);

//function to calculate the saturation
double calculate_saturation(double lightness, double max, double min);

double calculate_lightness(double max, double min);

colour rgb_colour(int r, int g, int b);

colour hsl_colour(int hue, double saturation, double lightness);

void colourChange(colour* c, int ret[]);


#endif
