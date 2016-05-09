#include <Servo.h>
Servo rightDrive;
Servo leftDrive;
int leftPwmPin  = 6;
int rightPwmPin = 7;
// Wheel Drive
int FWDML = 53;
int FWDMR = 51;
int RWDML = 52;
int RWDMR = 50;
// 0 = all, 1=front, 2=rear
int driveMode = 0;
//Motor Drive Functions
void allWheelDriveMode(){
  driveMode = 0;
  digitalWrite(FWDML, LOW);
  digitalWrite(FWDMR, LOW);
  digitalWrite(RWDML, LOW);
  digitalWrite(RWDMR, LOW);
}
void frontWheelDriveMode(){
  driveMode = 1;
  digitalWrite(FWDML, LOW);
  digitalWrite(FWDMR, LOW);
  digitalWrite(RWDML, HIGH);
  digitalWrite(RWDMR, HIGH);
}
void rearWheelDriveMode(){
  driveMode = 2;
  digitalWrite(FWDML, HIGH);
  digitalWrite(FWDMR, HIGH);
  digitalWrite(RWDML, LOW);
  digitalWrite(RWDMR, LOW);
}
void driveRight(float powerRatio){
  float multipler;
  int pwmValue;
  switch(driveMode){
    case 0:
      multipler = 1;
    break;
    default:
      multipler = 0.5;
    break;
  }
  pwmValue =  1500 + (int) (500*powerRatio*multipler);
  Serial.println("Right : " + String(pwmValue));
  rightDrive.writeMicroseconds(pwmValue);
}
void driveLeft(float powerRatio){
  float multipler;
  int pwmValue;
  switch(driveMode){
    case 0:
      multipler = 1;
    break;
    default:
      multipler = 0.5;
    break;
  }
  pwmValue =  1500 + (int) (500*powerRatio*multipler);
  Serial.println("Left : " + String(pwmValue));
  rightDrive.writeMicroseconds(pwmValue);
}

void setup(){
  Serial.begin(9600);
  delay(1000);
  Serial.println("Started");
  //Drive Motors
  pinMode(FWDML, OUTPUT);
  pinMode(FWDMR, OUTPUT);
  pinMode(RWDML, OUTPUT);
  pinMode(RWDMR, OUTPUT);
  rightDrive.attach(rightPwmPin);
  rightDrive.writeMicroseconds(1500);
  leftDrive.attach(leftPwmPin);
  leftDrive.writeMicroseconds(1500);
  Serial.println("ALL");
  allWheelDriveMode();
  driveRight(.5);
  driveLeft(.5);
  delay(3000);
  driveRight(-.5);
  driveLeft(-.5);
  delay(3000);
  driveRight(0);
  driveLeft(0);
  delay(5000);
  Serial.println("FRONT");
  frontWheelDriveMode();
  driveRight(.5);
  driveLeft(.5);
  delay(3000);
  driveRight(-.5);
  driveLeft(-.5);
  delay(3000);
  driveRight(0);
  driveLeft(0);
  delay(5000);
  Serial.println("REAR");
  rearWheelDriveMode();
  driveRight(.5);
  driveLeft(.5);
  delay(3000);
  driveRight(-.5);
  driveLeft(-.5);
  delay(3000);
  driveRight(0);
  driveLeft(0);
  Serial.println("End");
  
}
void loop(){
}

