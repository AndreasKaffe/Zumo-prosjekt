#include <ZumoShield.h>

//pins
const int LED = 13;
const int EchoPin1 = 6;
const int TrigPin1 = 2;
const int EchoPin2 = A1;
const int TrigPin2 = A0;


//states

const int S_RushDown = 0;
const int S_Search = 1;




//const values
const int fiveSeconds = 5000;
const int delay1 = 500;
const int delay2 = 500;

const int speedHigh = 350;
const int speedMid = 150;
const int speedLow = 100;
const int threshold = 1500;

//used variables
unsigned long nextTimeout;
int currentState = S_Search;
int rightDistance = 200;
int leftDistance = 200;
int right;
int left;
//Zumostuff
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

//sensors
const int NUM_SENSORS = 6;
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void setup()
{// put your setup code here, to run once:
  pinMode(LED, HIGH);
  pinMode(TrigPin1, OUTPUT);
  pinMode(EchoPin1, INPUT);
  countDown();
}

void loop()
{
  switch (currentState)
  {
    case(S_RushDown):
    
    right = speedLow;
    left = speedLow;
    leftDistance = LydDistance(TrigPin1, EchoPin1);
    rightDistance = LydDistance(TrigPin2, EchoPin2);
    if (leftDistance <= 100)
    {
      right = speedMid;
    
    }
    if (rightDistance <= 100)
    {
      left = speedMid;
      
    }
    motors.setSpeeds(right, left);
    if ((right == speedLow)&&(right == speedLow))
    {
      motors.setSpeeds(-speedLow, speedLow);
      currentState = S_Search;
    }
    break;
      
    case(S_Search):
    motors.setSpeeds(-speedLow,speedLow); //rotate on the spot
    leftDistance = LydDistance(TrigPin1,EchoPin1);
    rightDistance = LydDistance(TrigPin2, EchoPin2);
    if (((leftDistance) <= 100)||(rightDistance <= 100)) //oppdager et objekt innen 100 cm
    {
      motors.setSpeeds(speedMid, speedMid); //drive forward (should at triangulation function)
      currentState = S_RushDown;
    }
    break;
  }
  turnAtEdge();
  StoppButton();
}

void countDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
  delay(5000);
}

void turnAtEdge()
{
  sensors.read(sensor_values);

  
  if (sensor_values[5] < threshold)
  {
    motors.setSpeeds(-speedMid, -speedMid);
    delay(delay1);
    motors.setSpeeds(-speedMid, speedMid);
    delay(delay2);
    motors.setSpeeds(speedMid, speedMid);
  }
  else if (sensor_values[0] < threshold)
  {
    motors.setSpeeds(-speedMid, -speedMid);
    delay(delay1);
    motors.setSpeeds(speedMid, -speedMid);
    delay(delay2);
    motors.setSpeeds(speedMid, speedMid);
  }
}

void StoppButton()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    countDown();
  }
}

int LydDistance(int Trig, int Echo)
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long duration = pulseIn(Echo, HIGH);
  int distance = (duration * 0.034 / 2);
  return distance;
}
/*
 timer();// put your main code here, to run repeatedly:
 switch(currentState)
{
 case S_CountDown:
 timer();
 if (timeExpired() == true)
 {
  currentState = S_Drive;
 }
 break;
 case S_Drive:
 ZumoMotors myMotors;
 break;
 case S_Ram:
 
 break;
 case S_Edge:
 
 break;
 case S_dodge:
 
 break;
 
}*/
