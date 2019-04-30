#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define LEFT_SLOW 90
#define RIGHT_SLOW 107
#define LEFT_FAST 100
#define RIGHT_FAST 117
#define ENB 6
#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3

void setup() {
  Serial.begin(9600);
//  leftMotor(1);
//  rightMotor(1);
  Serial.println("IR1,IR2,IR3,IR4");
}

void loop() {

  int x [] = 
  {
    analogRead(A0),
    analogRead(A1),
    analogRead(A2),
    analogRead(A3)
  };

  int len = (sizeof(x)/sizeof(float));
  
  for(int i = 0; i < 4; i++)
  {
    Serial.print(x[i]);
    if(i != 3) Serial.print(", ");
    else Serial.println();
  }
//  if ( analogRead(IR4) > 200 ) // IF RIGHT IS WHITE
//  {
//    analogWrite(ENA, LEFT_SLOW); // SPEED RIGHT WHEEL
//    analogWrite(ENB, RIGHT_FAST);
//  }
//  else if ( analogRead(IR1) > 200 ) // IF LEFT IS WHITE
//  {
//    analogWrite(ENA, LEFT_FAST); // SPEED LEFT WHEEL
//    analogWrite(ENB, RIGHT_SLOW);
//  }
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

