#include "colour.h"
#include <stdio.h>

//https://www.rapidtables.com/convert/color/rgb-to-hsl.html rgb to hsl conversion refrence

//Function to move the value into the range 0 < x < 1
//expects a value that has some decimal component
double testCrush(double i)
{
	do
	{
		if (i < 0) i++;
		if (i > 1) i--;
	} while (i > 1 || i < 0);

	return i;
}

//tests the values of red, green and blue to calculate 
double test(double i, double tmp1, double tmp2)
{
	if (6 * i < 1)
	{
		i = tmp2 + (tmp1 - tmp2) * 6 * i;
	}
	else if (2 * i < 1)
	{
		i = tmp1;
	}
	else if (3 * i < 2)
	{
		i = tmp2 + (tmp1 - tmp2) * (0.666-i) * 6;
	}
	else
	{
		i = tmp2;
	}

	return i;
}

void maxMin(double* arr, double* max, double* min, char* rgb)
{
	*max = arr[0];
	int i;
	for (i = 0; i < 3; i++)
	{
		if (arr[i] > *max)
			*max = arr[i];
	}

	if (*max == arr[0])
		*rgb = 'r';
	else if (*max == arr[1])
		*rgb = 'g';
	else if (*max == arr[2])
		*rgb = 'b';

	*min = arr[0];
	for (i = 0; i < 3; i++)
	{
		if (arr[i] < *min)
			*min = arr[i];
	}
}

//function to calculate the hue of a colour given its red green and blue values
double calculate_hue(double* a, char rgb, double max, double min)
{
  const int r = 0, g = 1, b = 2;
	double h;
	switch (rgb)
	{
	case 'r': h = (a[g] - a[b]) / (max - min);
		break;
	case 'g': h = (2.0 + (a[b] - a[r]) / (max - min));
		break;
	case 'b': h = 4.0 + (a[r]- a[g]) / (max - min);
		break;
	}

  h *= 60;
    if (h < 0)
      h += 360;
	
	return h;
}

//function to calculate the saturation
double calculate_saturation(double lightness, double max, double min)
{
	double saturation;
	if (lightness < 0.5)
		saturation = (max - min) / (max + min);
	else
		saturation = (max - min) / (2.0 - max - min);

	return saturation;
}

double calculate_lightness(double max, double min)
{
	double lightness;
	lightness = (max + min) / 2.0;

	return lightness;
}

colour rgb_colour(int r, int g, int b)
{
	double max = 0, min = 0;
	char rgb;
	colour c = {(r/255.0), (g/255.0), (b/255.0)};
	double rgbArr[] = { c.red, c.green, c.blue };
	
	maxMin(rgbArr, &max, &min, &rgb); //find whether the value of r, g or b is the maximum value, returning the maximum and minimum values
	c.lightness = calculate_lightness(max, min);
	c.saturation = calculate_saturation(c.lightness, max, min);
	c.hue = calculate_hue(rgbArr, rgb, max, min);
	////std::cout << "hue: " << c.hue << std::endl;
	return c;
}

colour hsl_colour(int hue, double saturation, double lightness)
{
	colour* c;
	c->hue = hue;
	c->saturation = saturation;
	c->lightness = lightness;
	return *c;
}



void colourChange(colour* c, int ret[])
{
	double r, g, b;
	double tmp1, tmp2, hue;
	if (c->saturation < 0)
	{
		r = g = b = c->lightness * 255;
	}
	else
	{
		if (c->lightness < 0.5)
			tmp1 = c->lightness * (1.0 + c->saturation);
		else
		  tmp1 = c->lightness + c->saturation - c->lightness * c->saturation;
		
		tmp2 = 2 * c->lightness - tmp1;
		hue = c->hue / 360.0;
		r = hue + 0.333;
		g = hue;
		b = hue - 0.333;
		r = testCrush(r);
		g = testCrush(g);
		b = testCrush(b);
		r = test(r, tmp1, tmp2);
		g = test(g, tmp1, tmp2);
		b = test(b, tmp1, tmp2);
	}

	r *= 255;
	g *= 255;
	b *= 255;

  ret[0] = r;
  ret[1] = g;
  ret[2] = b;
	//std::cout << "R: " << r << ", G: " << g << ", B: " << b;
}
