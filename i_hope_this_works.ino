#include <Servo.h>


#define Rx 0
#define Tx
#define ENCL 2
#define ENCR 3
#define SERVO 9
#define LEFT 5
#define RIGHT 6
#define TRIG 7
#define IN1 8
#define IN2 4
#define IN3 10
#define IN4 11
#define ECHO 12
#define WALL 13
#define S1 AO
#define S2 A1
#define S3 A2
#define S4 A3
#define SDA A4
#define SCL A5

#define A0LINE 200//600
#define A1LINE 500//250
#define A2LINE 500//600
#define A3LINE 200//600

#define A1FULL 700
#define A2FULL 700
#define A1HALF 30
#define A2HALF 30

#define A0WHITE 1
#define A1WHITE 1
#define A2WHITE 1
#define A3WHITE 1

#define FUDGE 40
#define WALL_DIST 60

#define LEFT_MIN 75
#define RIGHT_MIN 90

#define LEFT_MAX 240
#define RIGHT_MAX 255

#define SPEED_SOUND 340.29 //meters per second
#define ECHO_MULTIPLIER 0.5
#define MICROSECONDS_IN_SECONDS 1e-6
#define MM_IN_M 1e3
#define MAX_TURN 180
#define DEF_DELAY 10

#define STOP 0

Servo ultrasonic;

struct Sensors {
  int ultrasonic;
  int ir[4];
} sensorData;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEFT, OUTPUT); //setting the pinmode for the pins provided
  pinMode(RIGHT, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(WALL, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(TRIG, LOW);
  ultrasonic.attach(SERVO);
  ultrasonic.write(90);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  Serial.begin(9600);
  int dist[2];

  ultrasonic.write(0);
  delay(500);
  dist[0] = ultrasonicRead();
  ultrasonic.write(180);
  delay(500);
  dist[1] = ultrasonicRead(); //left

  //follow_line();

  //collapsing_hallway(dist);


}

void readSensors()
{
  ultrasonic.write(90);
  delay(500);
  sensorData.ultrasonic = ultrasonicRead();
  sensorData.ir[0] = analogRead(A0);
  sensorData.ir[1] = analogRead(A1);
  sensorData.ir[2] = analogRead(A2);
  sensorData.ir[3] = analogRead(A3);
}

void readIR()
{
  sensorData.ir[0] = analogRead(A0);
  sensorData.ir[1] = analogRead(A1);
  sensorData.ir[2] = analogRead(A2);
  sensorData.ir[3] = analogRead(A3);
}

void loop() {
  while(true)
  {
    readIR();
    Serial.print(sensorData.ir[0]);
    Serial.print(", ");
    Serial.print(sensorData.ir[1]);
    Serial.print(", ");
    Serial.print(sensorData.ir[2]);
    Serial.print(", ");
    Serial.print(sensorData.ir[3]);
    Serial.print("\n");
  }
  // put your main code here, to run repeatedly

  readSensors();

  robot_stop();

  if (sensorData.ultrasonic < 20 && sensorData.ultrasonic != 0)
  {
    Serial.println("stop");
    Serial.println(sensorData.ultrasonic);
    robot_stop();
  }
  else if (sensorData.ir[1] > A1LINE || sensorData.ir[2] > A2LINE || sensorData.ir[3] > A3LINE || sensorData.ir[0] > A0LINE)
  {
    Serial.println("follow_line");
    follow_line();
  }
  else
  {
    Serial.println("check ultrasonic");
    ultrasonic.write(-10);
    delay(500);
    int tmp[2];
    tmp[0] = ultrasonicRead();
    ultrasonic.write(180);
    delay(500);
    tmp[1] = ultrasonicRead();

    if ((tmp[0] <= WALL_DIST && tmp[0] != 0) || (tmp[1] <= WALL_DIST && tmp[0] != 0)) //found wall follow it
    {
      Serial.println("follow_wall");
      Serial.print(tmp[0]);
      Serial.print(", ");
      Serial.print(tmp[1]);
      Serial.print("\n");
      follow_wall(WALL_DIST);
    }
    else
      {
      digitalWrite(IN1, LOW); //LEFT
      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, LOW); //RIGHT
      digitalWrite(IN4, HIGH);
      Serial.println("search");
      Serial.print(tmp[0]);
      Serial.print(", ");
      Serial.print(tmp[1]);
      Serial.print("\n");
      Serial.print(sensorData.ir[0]);
      Serial.print(", ");
      Serial.print(sensorData.ir[1]);
      Serial.print(", ");
      Serial.print(sensorData.ir[2]);
      Serial.print(", ");
      Serial.print(sensorData.ir[3]);
      Serial.print("\n");
      analogWrite(LEFT, LEFT_MIN);
      analogWrite(RIGHT, RIGHT_MIN);
      forward();
      }
  }


}

void forward()
{
  digitalWrite(IN1, LOW); //LEFT
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW); //RIGHT
  digitalWrite(IN4, HIGH);
  //set motors forward
  //run for a time

  analogWrite(LEFT, LEFT_MIN); //LEFT
  analogWrite(RIGHT, RIGHT_MIN); //RIGHT

  //left 75, right 90

  delay(10000);
}

void follow_line()
{
  //read sensors

  while (true)
  {
    int state;
    readIR();

    analogWrite(LEFT, LEFT_MIN); //LEFT
    analogWrite(RIGHT, RIGHT_MIN); //RIGHT

    Serial.print(sensorData.ir[0]);
    Serial.print(", ");
    Serial.print(sensorData.ir[1]);
    Serial.print(", ");
    Serial.print(sensorData.ir[2]);
    Serial.print(", ");
    Serial.print(sensorData.ir[3]);
    Serial.print("\n");

    if( sensorData.ir[1] <= A1LINE && sensorData.ir[2] <= A2LINE)
    {
        state = 0;//stop state
    }
    //detected left side of line    
    else if( sensorData.ir[2] <= A1LINE && sensorData.ir[1] > A1LINE )
    {
        state = 1;//turn left state
    }
    //detected right side of line
    else if( sensorData.ir[2] > A1LINE && sensorData.ir[1] <= A1LINE )
    {
        state = 2;//turn right state
    }
    //no line detected gogogo!
    else if( sensorData.ir[1] > A1LINE && sensorData.ir[2] > A2LINE )
    {
        state = 3;//go state
    }

    int startTime = millis();

    switch(state)
    {
    case 0://stop
      analogWrite(LEFT, 0);
      analogWrite(RIGHT, 0);
      break;
    case 1://turn left
      readIR();
      while(sensorData.ir[2] > A2LINE)
      {
          float speed = RIGHT_MIN + 0.1*(millis()-startTime);
          analogWrite(RIGHT, (char)speed);

          speed = LEFT_MIN - 0.1*(millis()-startTime);
          analogWrite(LEFT, (char)speed);
          readIR();    
      }
      break;
    case 2://turn right
      readIR();
      while(sensorData.ir[1] > A1LINE)
      {
           float speed = RIGHT_MIN - 0.1*(millis()-startTime);
          analogWrite(RIGHT, (char)speed);

          speed = LEFT_MIN + 0.1*(millis()-startTime);
          analogWrite(LEFT, (char)speed);
          readIR();
      }
      break;
    case 3://go
      analogWrite(LEFT, LEFT_MIN);
      analogWrite(RIGHT, RIGHT_MIN);
      break;
    default://stop

    break;
    }
    // ADJUST AND FOLLOW
    // if (sensorData.ir[0] > A0LINE) // LEFTMOST IS LINE
    // {
    //   digitalWrite(IN1, LOW); // LEFT STOP
    //   digitalWrite(IN2, LOW);
    //   digitalWrite(IN3, LOW); // RIGHT FORWARD
    //   digitalWrite(IN4, HIGH);
    // }
    // else if (sensorData.ir[3] > A3LINE) // RIGHTMOST IS LINE
    // {
    //   digitalWrite(IN1, LOW); // LEFT FORWARD
    //   digitalWrite(IN2, HIGH);
    //   digitalWrite(IN3, LOW); // RIGHT STOP
    //   digitalWrite(IN4, LOW);
    // }
    // else if (sensorData.ir[1] > A1LINE || sensorData.ir[2] > A2LINE)
    // {
    //   Serial.println("going forward");
    //   digitalWrite(IN1, LOW); // LEFT FORWARD
    //   digitalWrite(IN2, HIGH);
    //   digitalWrite(IN3, LOW); // RIGHT FORWARD
    //   digitalWrite(IN4, HIGH);
    // }
    // PREVIOUS
    // if (sensorData.ir[1] > A1LINE && sensorData.ir[2] > A2LINE)
    // {
    //   digitalWrite(IN1, LOW); //LEFT
    //   digitalWrite(IN2, HIGH);
    
    //   digitalWrite(IN3, LOW); //RIGHT
    //   digitalWrite(IN4, HIGH);
      
    //   analogWrite(LEFT, LEFT_MIN);
    //   analogWrite(RIGHT, RIGHT_MIN);
    //   //delay(200);
    // }
    // else if (sensorData.ir[1] > A1LINE) //right
    // {
    //   digitalWrite(IN1, LOW); //LEFT
    //   digitalWrite(IN2, HIGH);
    
    //   digitalWrite(IN3, HIGH); //RIGHT
    //   digitalWrite(IN4, LOW);
    //   analogWrite(LEFT, LEFT_MIN);
    //   analogWrite(RIGHT, RIGHT_MIN);
    //   //delay(100);
    // }
    // else if (sensorData.ir[1] > A1LINE && sensorData.ir[0] > A0LINE) //right
    // {
    //   analogWrite(LEFT, LEFT_MIN);
    //   analogWrite(RIGHT, RIGHT_MIN + FUDGE * 2);
    //   delay(50);
    // }
    // else if (sensorData.ir[2] > A2LINE) //left
    // {
    //   digitalWrite(IN1, HIGH); //LEFT
    //   digitalWrite(IN2, LOW);
    
    //   digitalWrite(IN3, LOW); //RIGHT
    //   digitalWrite(IN4, HIGH);
    //   analogWrite(LEFT, LEFT_MIN);
    //   analogWrite(RIGHT, RIGHT_MIN);
    //   //delay(100);
    // }
    // else if (sensorData.ir[2] > A2LINE && sensorData.ir[3] > A3LINE) //right
    // {
    //   analogWrite(LEFT, LEFT_MIN + FUDGE * 2);
    //   analogWrite(RIGHT, RIGHT_MIN);
    //   //delay(50);
    // }
    // else if (sensorData.ir[0] > A0LINE) //right
    // {
    //   analogWrite(LEFT, LEFT_MIN + FUDGE);
    //   analogWrite(RIGHT, 0);
    //   //delay(50);
    // }
    // else if (sensorData.ir[3] > A3LINE) //left
    // {
    //   analogWrite(LEFT, STOP);
    //   analogWrite(RIGHT, RIGHT_MIN + FUDGE);
    //   //delay(40);
    // }
    // else if (sensorData.ir[2] > A2LINE && sensorData.ir[1] > A1LINE) //on
    // {
    //   analogWrite(LEFT, LEFT_MIN);
    //   analogWrite(RIGHT, RIGHT_MIN);
    // }
    // else //off
    // {
    //   Serial.println("stopped");
    //   return;
    // }
    //int l = 85 + abs(dist * 5);
    //int r = 100 - abs(dist * 10);

    /*Serial.print(l);
      Serial.print(", ");
      Serial.print(r);
      Serial.print("\n");*/

    //analogWrite(LEFT, l);
    //analogWrite(RIGHT, r);

    //delay(20);

    //Serial.println(distToLine(ir));

  }
  //if left go right
  //if right go left
  //return
}

int distToLine(int* ir)
{
  double num = 0, denon = ir[0] + ir[1] + ir[2] + ir[3];

  num += ir[0] * 30;
  num += ir[1] * 10;
  num += ir[2] * -10;
  num += ir[3] * -30;

  return (int)ceil(num / denon);
}

unsigned int ultrasonic_getDist()
{
  //Serial.println("get dist");
  //code slightly modified from that found in the project refrence under ultrasonic distance sensor
  long duration = 0;
  // Read in a distance from the ultrasonic distance sensor:
  // The PING))) is triggered by a HIGH pulse of 10 microseconds.
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  //read length of time pulse
  duration = pulseIn(ECHO, HIGH, 1000); //duration in microseconds
  // convert the time into a distance
  return (unsigned int)(ECHO_MULTIPLIER * duration * MICROSECONDS_IN_SECONDS * SPEED_SOUND * MM_IN_M);
}

int ultrasonicRead()
{
  //Serial.println("read ultrasonic");
  unsigned int distances [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 10; i++)
  {
    //Serial.print("dist: ");
    //Serial.print(distances[i]);
    //Serial.print("\n");
    distances[i] = ultrasonic_getDist();
  }

  sort(distances);
  /*Serial.print("dist: ");
    Serial.print(distances[(int)ceil(sizeof(distances)/sizeof(unsigned int)/2)]);
    Serial.print("\n");*/

  return distances[(int)ceil(sizeof(distances) / sizeof(unsigned int) / 2)];
}

void sort(int* arr)
{
  //Serial.println("int sort");
  int len = sizeof(arr) / sizeof(int);

  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < len; j++)
    {
      if (arr[i] > arr[j])
      {
        int tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
      }
    }
  }
}

bool ultrasonic_isWall()
{
  if (ultrasonicRead() > WALL_DIST) return true;
  else return false;
}

bool ultrasonic_sweep()
{
  //Serial.println("begin sweep...");
  //Serial.begin(9600);
  ultrasonic.write(100);
  for (int pos = 0; pos < MAX_TURN; pos++)
  {
    ultrasonic.write(pos);
    delay(DEF_DELAY);
    //Serial.println(ultrasonic_getDist());
    if (ultrasonic_getDist() <= 600)
    {
      //Serial.print("found wall at: ");
      //Serial.print(pos);
      //Serial.print("\n");
      return true;
    }
  }
  return false;
}

void follow_wall(int dist)
{
  digitalWrite(IN1, LOW); //LEFT
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW); //RIGHT
  digitalWrite(IN4, HIGH);
  //find wall
  //if wall is left
  // get dist
  // if dist is > then go left
  // if dist is < then go right
  //if wall is right
  // get dist
  // if dist is < then go left
  // if dist is > then go right

  ultrasonic.write(0);//;ultrasonic_sweep();

  static int curr;

  curr = ultrasonicRead();



  while (ultrasonic_isWall())
  {
    curr = ultrasonicRead();

    Serial.println(curr);

    if (ultrasonic.read() < 90)
    {
      if (curr < dist)
      {
        analogWrite(LEFT, STOP);
        analogWrite(RIGHT, RIGHT_MIN);
      }
      else if (curr > dist)
      {
        analogWrite(LEFT, LEFT_MIN);
        analogWrite(RIGHT, STOP);
      }
      else
      {
        analogWrite(LEFT, LEFT_MIN);
        analogWrite(RIGHT, RIGHT_MIN);
        delay(100);
      }
    }
    else if (ultrasonic.read() > 90)
    {
      if (curr > dist)
      {
        analogWrite(LEFT, LEFT_MIN + FUDGE);
        analogWrite(RIGHT, RIGHT_MIN);
      }
      else if (curr < dist)
      {
        analogWrite(LEFT, LEFT_MIN);
        analogWrite(RIGHT, RIGHT_MIN + FUDGE);
      }
      /*else
        {
        analogWrite(LEFT, LEFT_MIN);
        analogWrite(RIGHT, RIGHT_MIN);
        }*/
    }

    //delay(50);

    /*analogWrite(LEFT, STOP);
      analogWrite(RIGHT, STOP);

      delay(500);*/

    //prev = curr;
  }
}

void collapsing_hallway(int* dist)
{
  //find wall
  //if wall is left
  // get dist
  // if dist is > then go left
  // if dist is < then go right
  //if wall is right
  // get dist
  // if dist is < then go left
  // if dist is > then go right

  ultrasonic.write(0);//;ultrasonic_sweep();

  static int curr[] = {dist[0], dist[1]};





  while (true)
  {
    ultrasonic.write(-10);
    delay(500);
    /*int tmp = ultrasonicRead();
      if(tmp < (dist[1]+dist[0])/2 + 10)*/
    curr[0] = ultrasonicRead(); //right
    ultrasonic.write(180);
    delay(500);
    /*tmp = ultrasonicRead();
      if(tmp < (dist[1]+dist[0])/2 + 10)*/
    curr[1] = ultrasonicRead(); //right

    //Serial.println(curr);

    if (curr[0] - curr[1] > 200) //if left go right
    {
      analogWrite(LEFT, STOP);
      analogWrite(RIGHT, RIGHT_MAX);
    }
    else if (curr[0] - curr[1] < -200) //if right go left
    {
      analogWrite(LEFT, LEFT_MAX);
      analogWrite(RIGHT, STOP);
    }
    else //if forward then go forward
    {
      analogWrite(LEFT, LEFT_MIN);
      analogWrite(RIGHT, RIGHT_MIN);
      delay(100);
    }
    delay(50);

    //Serial.print("difference: ");
    Serial.print(curr[0]);
    Serial.print(", ");
    Serial.print(curr[1]);
    Serial.print("\n");

    analogWrite(LEFT, STOP);
    analogWrite(RIGHT, STOP);

    delay(500);

    //prev = curr;
  }
}

void robot_stop()
{
  analogWrite(LEFT, 0);//stop
  analogWrite(RIGHT, 0);
}

void turnLeft()
{
  //turn 90 left
}

void turnRight()
{
  //turn 90 right
}
