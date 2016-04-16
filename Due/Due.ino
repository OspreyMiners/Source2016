#include <Servo.h>
#include <Encoder.h>
struct BiMotor{
  int sideA;
  int sideB;
};
//Pin Setup
//Output
// Wheel Drive
const int FWDML = 52;
const int FWDMR = 51;
const int RWDML = 50;
const int RWDMR = 49;
//Conveyors
const int Con1 = 53;
const int Con2 = 48;
const int Con3 = 47;
const int Con4 = 46;
const int Con5 = 45;
// Linear Actuator and Rack Pinion
BiMotor FrontLinearActuator = {44,43};
BiMotor RearLinearActuator = {42,41};
BiMotor RackPinionMotor = {40,39};
//Digger Motor
const int DiggerMotor = 38;
// PWM Drive Pins
const int RightPwmPin = 13;
const int LeftPwmPin = 12;
// Input
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
// Global Variables
//Current Arrays
const int currentSensorReadCount= 24;
int index = 0;
//Digger Relavant Data
float diggerCurrent[numReadings];// the index of the current reading
float totalDigger = 0;           // the running total
float avgDigger = 0;             // the average
float currentValDigger = 0;
//Dumper Relavant Data
float dumperCurrent[numReadings];
float totalDumper = 0;           // the running total
float avgDumper = 0;             // the average
float currentValDumper = 0;
//Actuator Variable Setup
//Drive Channels
Servo rightDrive;
Servo leftDrive;
//Drive Speeds
float speedRight = 0;
float speedLeft = 0;
// 0 = all, 1=front, 2=rear
int driveMode = 0;
//Motor Feedbacks
Encoder rackEncoder(RackEncoderA,RackEncoderB);
Encoder rightEncoder(RightEncoderA,RightEncoderB);
Encoder leftEncoder(LeftEncoderA,LeftEncoderB);
// Current Averages
int currentDigAvg = 0;
int currentConAvg = 0;
int linActFeedback = 0;
String inputString = ""; 
//Robot Setup 
void startUpProcedure(){
  //Reserve 16 bytes for input
  inputString.reserve(16);
  //inputs are not set as arduino defaults pins to input
  // Setup Actuators
  //Drive Motors 
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
  //Digger and Conveyor Motors
  pinMode(Con1, OUTPUT);
  pinMode(Con2, OUTPUT);
  pinMode(Con3, OUTPUT);
  pinMode(Con4, OUTPUT);
  pinMode(Con5, OUTPUT);
  pinMode(DiggerMotor, OUTPUT);
  //Linear Acutators and Rack Pinion Motor
  pinMode(FrontLinearActuator.sideA, OUTPUT);
  pinMode(FrontLinearActuator.sideB, OUTPUT);
  pinMode(RearLinearActuator.sideA, OUTPUT);
  pinMode(RearLinearActuator.sideB, OUTPUT);
  pinMode(RackPinionMotor.sideA, OUTPUT);
  pinMode(RackPinionMotor.sideB, OUTPUT);
  //bind limit switch
  attachInterrupt(digitalPinToInterrupt(LimitSwitchPin), stopRack, HIGH);
  prepDigger();
  for (int thisReading = 0; thisReading < currentSensorReadCount; thisReading++){
    dumperCurrent[thisReading] = 0;
    diggerCurrent[thisReading] = 0;
  }
}
void haltRobot(){
  noInterrupts();
  //Halt Drive
  rightDrive.writeMicroseconds(1500);
  leftDrive.writeMicroseconds(1500);
  //stop Conveyors and digger
  stopConveyorsAndDigger();
  speedLeft = 0;
  speedRight = 0;
  interrupts();
  inputString = "";
}
void resetRobot(){
  haltRobot();
  positionDigger(0);
  angleDigger(10);
  Serial.println("O:");
}
void prepDigger(){
  angleDigger(10);
  biDirectMotorReverse(RackPinionMotor);
}
//Main code
void setup() {
  Serial.begin(9600);
  startUpProcedure();
}
void loop() {
  //Collect Data
  //Collect Current Sensors
    //Check Within Bounds
  //Report Data
    //Report Quad Encoders
  //Motor PID Loop
   //apply PI loop
}
//End Of Main
//Serial TX
void collectAndReport(){
  
}
//Current Sensor Collect
float senseDumperCurrent(){
  float sensitivity = 0.04 
  //Digger Relavant Data
  totalDumper = totalDumper - dumperCurrent[index];
  dumperCurrent[index] = analogRead(CurrentDig);
  dumperCurrent[index] = (dumperCurrent[index]-512)*5/1024/sensitivity-sensitivity;
  totalDumper = totalDumper + diggerCurrent[index];
  if (index >= numReadings)              
      index = 0;                           
  avgDigger = totalDumper/currentSensorReadCount;
  currentValDigger = avgDigger;
  return currentValDigger;
  float diggerCurrent[numReadings];// the index of the current reading
  float totalDigger = 0;           // the running total
  float avgDigger = 0;             // the average
  float currentValDigger = 0;
  //Dumper Relavant Data
  float dumperCurrent[numReadings];
  float totalDumper = 0;           // the running total
  float avgDumper = 0;             // the average
  float currentValDumper = 0;
}
float senseDiggerCurrent(){
  float sensitivity = 0.04 
  //Digger Relavant Data
  totalDumper = totalDumper - diggerCurrent[index];
  diggerCurrent[index] = analogRead(CurrentCon);
  diggerCurrent[index] = (diggerCurrent[index]-512)*5/1024/sensitivity-sensitivity;
  totalDumper = totalDumper + diggerCurrent[index];
  if (index >= currentSensorReadCount)              
      index = 0;                          
  avgDigger = totalDigger/currentSensorReadCount;
  currentValDigger = avgDigger;
  
  float diggerCurrent[numReadings];// the index of the current reading
  float totalDigger = 0;           // the running total
  float avgDigger = 0;             // the average
  float currentValDigger = 0;
  //Dumper Relavant Data
  float dumperCurrent[numReadings];
  float totalDumper = 0;           // the running total
  float avgDumper = 0;             // the average
  float currentValDumper = 0;
}
//Parser
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inputString = Serial.readStringUntil(' ');
    //ID Command Type by the first character
    switch(inputString.charAt(0)){
      case 'C':
        handleCommandC(inputString);
      break;
      case 'L':
        handleCommandL(inputString);
        break;
      case 'P':
        handleCommandP(inputString);
        break;
      case 'D':
        handleCommandD(inputString);
        break;
      case 'W':
        handleCommandW(inputString);
        break;
      case 'M':
        handleCommandM(inputString);
        break;
      case 'H':
        handleCommandH(inputString);
        break;
      case 'N':
        handleCommandN(inputString);
        break;
      default:
        Serial.println("Failure:NotValidCommand");
        break;
      
    }
    inputString = "";
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
//Feedback Functions
double angleReadOut(int value){
  value -= 100;
  double reading = 1 - value/915.0;
  double s = reading * 6;
  s = s + 12;
  return 2 * atan((275 - (4*sqrt((-4 * pow(s, 4)) + (2965 * pow(s, 2)) - 342225))) / ((8 * pow(s, 2)) - 4765)) * (180 / 3.14159f);
}
//Control Functions
//Abstraction Functions
void handleCommandW(String inputString){
  switch(inputString.charAt(1)){
    case 'A':
      driveMode = 0;
      break;
    case 'F':
      driveMode = 1;
      break;
    case 'R':
      driveMode = 2;
      break;
    default :
      Serial.println("Failure: Invalid Drive Mode");
      break;
  }
}
void handleCommandH(String inputString){
  haltRobot();
}
void handleCommandN(String inputString){
  resetRobot();
}
void handleCommandP(inputString){
  float targetAngle = inputString.Substring(1).toInt(); 
  angleDigger(targetAngle);
}
void handleCommandD(inputString){
  float targetDistance = inputString.Substring(1).toInt(); 
  positionDigger(targetDistance);
}
void handleCommandL(inputString){
  switch(inputString.charAt(1)){
    case '+':
      biDirectMotorForward(RearLinearActuator);
      break;
    case '-':
      biDirectMotorReverse(RearLinearActuator);
      break;
    default:
  }
}
void handleCommandM(String inputString){
  float motorPowerRatio = inputString.subString(3).toInt()/100.0;
  if(inputString.charAt(2) == '-'){
    motorPowerRatio *= -1;
  }
  if(inputString.charAt(1) == '1'){
    driveRight(motorPowerRatio);
  }
  else{
    driveLeft(motorPowerRatio);
  }
}
void handleCommandC(String inputString){
  //Assume 2nd byte is + or -
  //Check Length of the string to see if all command or single
  bool all = false;
  switch(inputString.charAt(1)){
    case '+':
      turnConveyorsOn();
      break;
    case '-':
      turnConveyorsOff();
      break;
    default:
      //Assume that charAt(1) is a #
      {
        String number = inputString.substring(1);
        bool on = false;
        //Check for + or -
        if(number.indexOf('+') != -1){
          int endChar = number.indexOf('+');
          switch(number.substring(0,endChar).toInt()){
            case 1:
              turnMotorOn(Con1);
              break;
            case 2:
              turnMotorOn(Con2);
              break;
            case 3:
              turnMotorOn(Con3);
              break;
            case 4:
              turnMotorOn(Con4);
              break;
            case 5:
              turnMotorOn(Con5);
              break;
            case 6:
              turnMotorOn(DiggerMotor);
              break;
            default:
              Serial.println("Failure: Invalid Conveyor/Digger Motor #");
              break;
          }
        }
        else if(number.indexOf('-') != -1){
          int endChar = number.indexOf('+');
          switch(number.substring(0,endChar).toInt()){
            case 1:
              turnMotorOff(Con1);
              break;
            case 2:
              turnMotorOff(Con2);
              break;
            case 3:
              turnMotorOff(Con3);
              break;
            case 4:
              turnMotorOff(Con4);
              break;
            case 5:
              turnMotorOff(Con5);
              break;
            case 6:
              turnMotorOff(DiggerMotor);
              break;
            default:
              Serial.println("Failure: Invalid Conveyor/Digger Motor #");
              break;
          }
        }
}

//Digger funtionality
void stopConveyorsAndDigger(){
  turnMotorOff(Con1);
  turnMotorOff(Con2);
  turnMotorOff(Con3);
  turnMotorOff(Con4);
  turnMotorOff(Con5);
  turnMotorOff(DiggerMotor);
}
void startConveyorsAndDigger(){
  turnMotorOn(Con1);
  turnMotorOn(Con2);
  turnMotorOn(Con3);
  turnMotorOn(Con4);
  turnMotorOn(Con5);
  turnMotorOn(DiggerMotor);
}
void stopRack(){
  biDirectMotorOff(RackPinionMotor);
  rackEncoder.write(0);
}
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
void positionDigger(float goalLength){
  float pinionDiameter = 1.1 ;// inches
  int encoderTicksPerTurn = 360;
  float errorRange = .125;
  double distance = 3.14 * pinionDiameter / encoderTicksPerTurn * rackEncoder.read();
  while(goalLength + errorRange < distance || goalLength - errorRange > distance){
    if(goalLength > distance){
      biDirectMotorReverse(RackPinionMotor);
    }
    else if(goalLength < distance){
      biDirectMotorForward(RackPinionMotor);
    }
    distance = 3.14 * pinionDiameter / encoderTicksPerTurn * rackEncoder.read();
  }
  biDirectMotorOff(RackPinionMotor);
  //print out to serial hit event and value at
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