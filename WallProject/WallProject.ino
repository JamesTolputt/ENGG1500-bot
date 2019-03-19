// Encoder library
#include <ENGG1500Lib.h>

// Display Libraries
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> // SEARCH FOR THIS LIBRARY TO PRINT TO SCREEN

// Servo Library
#include <Servo.h>

// Motor Pins
#define ENA 5 // LEFT
#define ENB 6 // RIGHT
#define IN1 8
#define IN2 4
#define IN3 10
#define IN4 11

// Sonic Sensor Pins
#define TRIG 7
#define ECHO 12

// Motor directions
enum direction{
  forward, backward, noward
};

// Servo variables
#define SERVO 9
#define SERVO_RIGHT 0
#define SERVO_CENTER 90
#define SERVO_LEFT 180
Servo sonar_servo;

// Screen Constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Screen object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function Prototypes (for parameter defaults and readablity)
void left_motor(direction dir = forward);
void right_motor(direction dir = forward);
void draw_smiley(void);
unsigned int sonar_distance(byte checks=10);
bool sense_left(void);
bool sense_right(void);

// THIS PROJECT ONLY
enum state {
  parking, finding_wall, driving
};
state current_state = finding_wall;

//START
void setup() {
  // Setup pins
  //// Motor Pins
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(IN1,OUTPUT); //set IN1 as an output
  pinMode(IN2,OUTPUT); //set IN2 as an output
  pinMode(IN3,OUTPUT); //set IN3 as an output
  pinMode(IN4,OUTPUT); //set IN4 as an output
  //// IR Sensor pins
  pinMode(A2, INPUT); //right sensor
  pinMode(A3, INPUT); //left sensor
  //// Sonic Sensor pins
  pinMode(ECHO,INPUT);
  pinMode(TRIG,OUTPUT);
  //// Servo pin
  sonar_servo.attach(9); // Makes sonar_servo object use pin 9

  // Begin serial to print to console (disables pins 1 and 0)
  Serial.begin(9600); //set the baud rate on the serial connection
  // Initialise encoder library
  enc_init();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Initialise display if failed...
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } else { // Else draw smiley to confirm nothing went wrong
    draw_smiley();
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    sonar_servo.write(SERVO_CENTER);
  }
  //delay(5000);//delay before starting loop

  Serial.println(sense_right());
  if (sense_right() and sense_left()) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("We're Here");
    display.display();
    for(;;);
  }
//  } else if (sense_right() or sense_left())
//    
//  } else {
//    
//  }
}

void loop()
{
  // Clear encoder as the robot hasn't moved yet
  enc_clear();

  // Set motors to forward
  left_motor(); //set the left motor to run forwards
  right_motor(); //set the right motor to run forwards

//  state next_state = current_state;
//  switch(state) {
//    case driving:
//    next_state = driving();
//    break;
//    case finding_wall:
//    next_state = find_wall();
//    break;
//    case parking:
//    next_state = park();
//    break;
//  }
//  current_state = next_state;

//  // PSEUDOCODE FOR PROJECT
//  BEGIN
//    IF sense for square is true THEN
//      park the robot on square
//      STOP
//    ELSE
//      WHILE sense for square is false THEN
//        find a wall and hug it
//        drive along wall
//      ENDWHILE
//      park the robot on square
//    ENDIF
//  END
  
  
//  while((enc_getLeft()+enc_getRight())/2 <= 20) {
//    Serial.print("Encoder values:");Serial.print(enc_getLeft());Serial.print(", ");Serial.println(enc_getRight());
//    Serial.println((enc_getLeft()+enc_getRight())/2);
//    analogWrite(ENA, 110);
//    analogWrite(ENB, 150);
//    if (enc_getLeft() >= 20)
//      analogWrite(ENA, 0);////leftVel); //circle
//    if (enc_getRight() >= 20)
//      analogWrite(ENB, 0);////rightVel); //circle
//  }
//
//  analogWrite(ENA, 0);
//  analogWrite(ENB, 0);
//  delay(1000);
}

//This function sets IN1 = LOW and IN2 = HIGH in order to set the direction to forwards for motor 1
void left_motor(direction dir) {
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
//This function sets IN3 = LOW and IN4 = HIGH in order to set the direction to forwards for motor 2
void right_motor(direction dir) {
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
// This function draws the initial smiley face
void draw_smiley(void) {
  display.clearDisplay();
  display.fillCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, WHITE);
  display.fillCircle(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-(SCREEN_HEIGHT/12), SCREEN_HEIGHT/2+(SCREEN_HEIGHT/32), BLACK);
  display.fillCircle((SCREEN_WIDTH/2)-(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
  display.fillCircle((SCREEN_WIDTH/2)+(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
  display.display();
}
// This function outputs the averaged distance from the ultrasonic sensor to an obstacle
const float conversion = 0.170145; // is (0.5 * 1e-6 * 1e3 * 340.29(speed of sound))
                                   // 0.5 = half the distance so it's not the distance back and forth
                                   // 1e-6 = microseconds to seconds conversion (duration is in microseconds)
                                   // 1e3 = conversion from metres (speed of sound in metres) to mm
unsigned int sonar_distance(byte checks) {
  long average_duration = 0;
  for (byte i = 0; i < checks; i++) {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    average_duration += pulseIn(ECHO, HIGH);
  }
  average_duration /= checks;
  return (unsigned int)(average_duration * conversion);
}

bool sense_left(void) {
  if (digitalRead(A3) == 1) // return true if black
    return true;
  else return false;
}

bool sense_right(void) {
  if (digitalRead(A2) == 1)
    return true;
  else return false;
}
//END
