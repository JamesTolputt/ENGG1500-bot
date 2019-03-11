
#include "colour.h"

#define RED_2 10
#define GREEN_2 8
#define BLUE_2 9

#define RED 13
#define GREEN 11
#define BLUE 12

colour blue = rgb_colour(0, 0, 255);
colour red = rgb_colour(255, 0, 0);
colour green = rgb_colour(0, 255, 0);
colour yellow = rgb_colour(150, 100, 0);
colour aqua = rgb_colour(0, 255, 1);
colour pink = rgb_colour(255, 10, 10);
colour purple = rgb_colour(125, 0, 125);
colour orange = rgb_colour(255, 50, 0);

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED_2, OUTPUT);
  pinMode(GREEN_2, OUTPUT);
  pinMode(BLUE_2, OUTPUT);
  Serial.begin(9600);  
}

void loop() {
  //colourOut_1(blue);
  cycle_2(30);
  cycle_1(5);
}

void cycle_1(int time)
{
  static int ctr = 100;
  static int i;
  if(i > 0)
  {
    
    // put your main code here, to run repeatedly:
    colour c = hsl_colour(ctr, 1, 0.1);
    
    int a[3];
    colourChange(&c, a);
    output_1(a);
    i--;
  }
  else
    i = time;
  
  ctr++;
}

void cycle_2(int time)
{
  static int ctr = 100;
  static int i;
  if(i > 0)
  {
    
    // put your main code here, to run repeatedly:
    colour c = hsl_colour(ctr, 1, 0.1);
    
    int a[3];
    colourChange(&c, a);
    output_2(a);
    i--;
  }
  else
    i = time;
  
  ctr++;
}

void colourOut_1(colour c)
{    
    int a[3];
    colourChange(&c, a);
    output_1(a);
}

void colourOut_2(colour c)
{    
    int a[3];
    colourChange(&c, a);
    output_2(a);
}

void output_1(int a[])
{
    
	if(a[0] > 0)
	{
	  digitalWrite(RED, HIGH);
    delayMicroseconds(a[0]);
    digitalWrite(RED, LOW);
    delayMicroseconds(255-a[0]);
	}
	if(a[1] > 0)
  {
  	digitalWrite(GREEN, HIGH);
  	delayMicroseconds(a[1]);
  	digitalWrite(GREEN, LOW);
  	delayMicroseconds(255-a[1]);
  }

  if(a[2] > 0)
  {
  	digitalWrite(BLUE, HIGH);
  	delayMicroseconds(a[2]);
  	digitalWrite(BLUE, LOW);
  	delayMicroseconds(255-a[2]);
  }
}

void output_2(int a[])
{
    
  if(a[0] > 0)
  {
    digitalWrite(RED_2, HIGH);
    delayMicroseconds(a[0]);
    digitalWrite(RED_2, LOW);
    delayMicroseconds(255-a[0]);
  }
  if(a[1] > 0)
  {
    digitalWrite(GREEN_2, HIGH);
    delayMicroseconds(a[1]);
    digitalWrite(GREEN_2, LOW);
    delayMicroseconds(255-a[1]);
  }

  if(a[2] > 0)
  {
    digitalWrite(BLUE_2, HIGH);
    delayMicroseconds(a[2]);
    digitalWrite(BLUE_2, LOW);
    delayMicroseconds(255-a[2]);
  }
}
