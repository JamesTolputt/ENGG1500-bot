#include <ENGG1500Lib.h>

#define ENA 5 // LEFT
#define ENB 6 // RIGHT
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11


#define LEFT 1
#define RIGHT 0
enum direction{
  forward, backward, noward
};

//START
void setup() {
  // put your setup code here, to run once:
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(IN1,OUTPUT); //set IN1 as an output
  pinMode(IN2,OUTPUT); //set IN2 as an output
  pinMode(IN3,OUTPUT); //set IN3 as an output
  pinMode(IN4,OUTPUT); //set IN4 as an output
  pinMode(A0, INPUT); //right sensor
  pinMode(A1, INPUT); //left sensor
  Serial.begin(9600); //set the baud rate on the serial connection
  enc_init();
  delay(5000);//delay before starting loop
}

void loop()
{
  enc_clear();
  // put your main code here, to run repeatedly:
  left(forward); //set the left motor to run forwards
  right(forward); //set the right motor to run forwards
  ////int sensorVal[] = {digitalRead(A0), digitalRead(A1)}; //1 if no obstacle is present otherwise 0  
  
  //let’s say our desired speeds are 255 and 180
  //multiply the previous by the desired speed (* is multiply)
  ////int leftVel = sensorVal[LEFT]*180;
  ////int rightVel = sensorVal[RIGHT]*180;
  while((enc_getLeft()+enc_getRight())/2 < 50) {
    analogWrite(ENA,200);
    analogWrite(ENB, 200);
    if (enc_getLeft() >= 25)
      analogWrite(ENA, 0);////leftVel); //circle
    if (enc_getRight() >= 25)
      analogWrite(ENB, 0);////rightVel); //circle
  }

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  delay(1000);
  
  ////Serial.print("Left  ");Serial.print(leftVel);Serial.print(sensorVal[LEFT]);Serial.print("\n");
  ////Serial.print("Right ");Serial.print(rightVel);Serial.print(sensorVal[RIGHT]);Serial.print("\n");
}

void left(direction dir) //This function sets IN1 = LOW and IN2 = HIGH in order to set the direction to forwards for motor 1
{
  //Serial.println("forward");
  if(dir == forward)
  {
    digitalWrite(IN1,LOW); //IN1
    digitalWrite(IN2,HIGH); //IN2
  }
  else if(dir == backward)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if(dir == noward)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  
}

void right(direction dir)  //This function sets IN3 = LOW and IN4 = HIGH in order to set the direction to forwards for motor 2
{
  //Serial.println("backward");
  if(dir == forward)
  {
    digitalWrite(IN3,LOW); //IN3
    digitalWrite(IN4,HIGH); //IN4
  }
  else if(dir == backward)
  {
    digitalWrite(IN3,HIGH); //IN3
    digitalWrite(IN4,LOW); //IN4
  }
  else if(dir == noward)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}
//END
