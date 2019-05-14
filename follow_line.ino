#include <Servo.h>
// OUTPUT
#define ENA 5
#define ENB 6
#define IN1 8
#define IN2 4
#define IN3 10
#define IN4 11
#define TRIG 7
#define SERVO 9 

// INPUT
#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define ECHO 12

// Constants
#define LEFT_SLOW 90
#define RIGHT_SLOW 107

Servo servo;

int irRead( int pin, int samples = 5 );

void setup() 
{
  // OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  servo.attach(SERVO);

  // INPUT
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
  leftMotor(1);
  rightMotor(1);
  Serial.println("IR1,IR2,IR3,IR4");
}

void loop() {
//  // SENSOR READING CODE
//  int x [] = 
//  {
//    irRead(IR1),
//    irRead(IR2),
//    irRead(IR3),
//    irRead(IR4)
//  };
//
//  int len = ( sizeof(x) / sizeof(float) );
//  
//  for( int i = 0; i < 4; i++ )
//  {
//    Serial.print(x[i]);
//    if(i != 3) Serial.print(", ");
//    else Serial.println();
//  }
  // MOTOR MOVING CODE
  
  if ( irRead(IR1) >  150 )          // IF LEFT IS BLACK
  {
    analogWrite(ENA, 200);           // SPEED RIGHT WHEEL
    analogWrite(ENB, LEFT_SLOW) ;    // SLOW LEFT WHEEL
    delay(50);                       // DELAY FOR A SIGNIFICANT CORRECTION
  }
  else if ( irRead(IR4) > 150 )      // IF  RIGHT IS BLACK
  {
    analogWrite(ENA, RIGHT_SLOW);    // SPEED RIGHT WHEEL
    analogWrite(ENB, 200);           // SLOW LEFT WHEEL
    delay(50);                       // DELAY FOR A SIGNIFICANT CORRECTION
  }
  else if ( irRead(IR2) > 30 && irRead(IR3) > 30) // IF BOTH INTERNAL IR SENSORS SEES BLACK LINE
  {
    analogWrite(ENA, RIGHT_SLOW);    // MOVE MINIMUM SPEED 
    analogWrite(ENB, LEFT_SLOW);     // 
  }
  else // NO SENSORS SEE BLACK LINE
  {
    // STATE SWAP!
    analogWrite(ENA, 0); // TURN OFF MOTORS
    analogWrite(ENB, 0);
  }
}

void leftMotor(int dir)
{
  if (dir == 1) //FORWARDS
  {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
  }
  else if (dir == -1) //BACKWARDS
  {
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
  }
  else //STOP
  {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
  }
}

void rightMotor(int dir)
{
  if (dir == 1) //FORWARDS
  {
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
  }
  else if (dir == -1) //BACKWARDS
  {
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
  }
  else //STOP
  {
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
  }
}

/** READS THE IR SENSOR samples TIMES
 ** AND AVERAGES THE RESULT
 ** ASSURES VALUE IS BETWEEN 0 and 255
 */ 
int irRead( int pin, int samples )
{
  int value; // CUMULATIVE VALUE TO SUM BEFORE AVERAGE
  for ( int i = 0; i < samples; i++ ) // FOR SAMPLE SIZE...
  {
    value += map( analogRead(pin), 0, 1023, 0, 255); // ADD TO VALUE BUT MAKE SURE VALUE IS BETWEEN 0 AND 255
  }
  return (char) value / samples; // AVERAGE
}
