#include <ZumoShield.h>

//pins
const int LED = 13;

//states
const int S_CountDown = 0;
const int S_Drive = 1;
const int S_Ram = 2;
const int S_Edge = 3;
const int S_dodge = 4;

//const values
const int fiveSeconds = 5000;
const int oneFifth = 200;
const int threeTenths = 300;

const int speedHigh = 350;
const int speedMid = 200;
const int speedLow = 100;
const int threshold = 1500;

//used variables
unsigned long nextTimeout;
int currentState = S_CountDown;

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
  
  
  countDown();
}

void loop()
{
  sensors.read(sensor_values);

  turnAtEdge();

  motors.setSpeeds(speedMid, speedMid);
  
}

void countDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);

  delay(5000);
}

void timer(int wait)
{
  unsigned long currentTime = millis();
  nextTimeout = currentTime + wait;
}
bool timeExpired()
{
  unsigned long currentTime = millis();
  if(currentTime < nextTimeout)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void turnAtEdge()
{
  
  if (sensor_values[0] > threshold)
  {
    motors.setSpeeds(-speedMid, -speedMid);
    delay(oneFifth);
    motors.setSpeeds(-speedMid, speedMid);
    delay(threeTenths);
    motors.setSpeeds(speedMid, speedMid);
  }
  else if (sensor_values[5] > threshold)
  {
    motors.setSpeeds(-speedMid, -speedMid);
    delay(oneFifth);
    motors.setSpeeds(speedMid, -speedMid);
    delay(threeTenths);
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
