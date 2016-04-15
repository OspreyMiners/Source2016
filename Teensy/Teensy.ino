#include <Servo.h>
#include <Encoder.h>


struct BiMotor{
  int sideA;
  int sideB;
};
// Output Pin setup
// Wheel Drive
const int FWDML = 5;
const int FWDMR = 6;
const int RWDML = 7;
const int RWDMR = 8;
// PWM Drive Pins
Servo rightDrive;
Servo leftDrive;
const int RightPwmPin = 3;
const int LeftPwmPin = 4;
// Linear Actuator
BiMotor FrontLinearActuator = {9,10};
BiMotor RearLinearActuator = {11,12};
BiMotor RackPinionMotor = {13,14};
// Conveyor Motors
const int Con1 = 15;
const int Con2 = 16;
const int Con3 = 17;
const int Con4 = 18;
const int Con5 = 19;

// Input Pin Setup
// Current sensor feedback
const int CurrentDig = A6;
const int CurrentCon = A7;
// LinearActuator feedback
const int FLAPotFeedback = A8;
// Limit Switch
const int LimitSwitchPin= 23;
// Encoders feedback
const int RightEncoderA = 24;
const int RightEncoderB = 25;
const int LeftEncoderA = 32;
const int LeftEncoderB = 33;
const int RackEncoderA = 26;
const int RackEncoderB = 31;
Encoder rackEncoder(RackEncoderA,RackEncoderB);
Encoder rightEncoder(RightEncoderA,RightEncoderB);
Encoder leftEncoder(LeftEncoderA,LeftEncoderB);

// Global Variables
// 0 = all, 1=front, 2=rear
int driveMode = 0;
// Encoder Counts
int rackPinionCount = 0;
int rightCount = 0;
int leftCount = 0;
// Current Averages
int currentDigAvg = 0;
int currentConAvg = 0;
void startUp(){
  // Setup Drive Motors
  pinMode(FWDML, OUTPUT);
  pinMode(FWDMR, OUTPUT);
  pinMode(RWDML, OUTPUT);
  pinMode(RWDMR, OUTPUT);
  rightDrive.attach(RightPWMPin);
  rightDrive.writeMicroseconds(1500);
  lefttDrive.attach(LeftPWMPin);
  lefttDrive.writeMicroseconds(1500);
  // Initialize All motors to off
  turnMotorOff(FWDML);
  turnMotorOff(FWDMR);
  turnMotorOff(RWDML);
  turnMotorOff(RWDMR);
  //bind encoders

  //bind limit switch
  attachInterrupt(digitalPinToInterrupt(LimitSwitchPin), stopRack, LOW);
  prepDigger();
}
void prepDigger(){
  biDirectMotorReverse(FrontLinearActuator);
  biDirectMotorReverse(RackPinionMotor);
}
//Main code
void setup() {
  rightDrive.begin(RightPwmPin);
  leftDrive.begin(LeftPwmPin);
  Serial.begin(9600);
}
void loop() {
  rightDrive.write(1,1000);
  leftDrive.write(1,1000);
  Serial.println(0);
  delay(1000);
  rightDrive.write(1,1500);
  leftDrive.write(1,1500);
  Serial.println(1024);
  delay(1000);
  rightDrive.write(1,2000);
  leftDrive.write(1,2000);
  Serial.println(2047);
  delay(1000);
}
//End Of Main
//Digger funtionality
void angleDigger(float goalAngle){
  double sensorValue = angleReadOut(analogRead(FLAPotFeedback));
  while((goalAngle + .125 < sensorValue ) || (goalAngle - .125 > sensorValue)){
    if(goalAngle > sensorValue){
      biDirectMotorForward(FrontLinearActuator);
    }
    else if(goalAngle < sensorValue){
      biDirectMotorReverse(FrontLinearActuator);
    }
    else{
      biDirectMotorOff(FrontLinearActuator);
    }
    sensorValue = angleReadOut(analogRead(FLAPotFeedback));
  }
  biDirectMotorOff(FrontLinearActuator);
  //print out to serial hit event and value at
}
double angleReadOut(int value){
  value -= 100;
  double reading = 1 - value/915.0;
  double s = reading * 6;
  s = s + 12;
  return 2 * atan((275 - (4*sqrt((-4 * pow(s, 4)) + (2965 * pow(s, 2)) - 342225))) / ((8 * pow(s, 2)) - 4765)) * (180 / 3.14159f);
}
void positionDigger(float goalLength){
  float pinionDiameter;
  int encoderTicksPerTurn;
  float errorRange = .125;
  double distance = 3.14 * pinionDiameter / encoderTicksPerTurn * rackPinionCount;
  while(goalLength + errorRange < distance || goalLength - errorRange > distance){
    if(goalLength > distance){
      biDirectMotorReverse(RackPinionMotor);
    }
    else if(goalLength < distance){
      biDirectMotorForward(RackPinionMotor);
    }
    distance = 3.14 * pinionDiameter / encoderTicksPerTurn * rackPinionCount;
  }
  biDirectMotorOff(RackPinionMotor);
  //print out to serial hit event and value at
}
void stopRack(){
  biDirectMotorOff(RackPinionMotor);
  rackPinionCount = 0;
}
//Motor Drive Functions
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
  pwmValue =  1500 + 500*powerRatio*multipler;
  analogWrite(RightPwmPin,pwmValue);
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
  pwmValue =  1500 + 500*powerRatio*multipler;
  analogWrite(LeftPwmPin,pwmValue);
}
// Linear Actuator & Rack Pinion Control Functions
void biDirectMotorForward(BiMotor motor){
  digitalWrite(motor.sideA, HIGH);
  digitalWrite(motor.sideB, LOW);
}
void biDirectMotorReverse(BiMotor motor){
  digitalWrite(motor.sideA, LOW);
  digitalWrite(motor.sideB, HIGH);
}
void biDirectMotorOff(BiMotor motor){
  digitalWrite(motor.sideA, HIGH);
  digitalWrite(motor.sideB, HIGH);
}

// Motor Control Functions
void turnMotorOff(int pinNumber){
  digitalWrite(pinNumber, HIGH);
}
void turnMotorOn(int pinNumber){
  digitalWrite(pinNumber, LOW);
}

// Drive Mode Functions
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
