#include <Servo.h>
#include <Encoder.h>
typedef struct BiMotor{
	int sideA;
	int sideB;
};
//Pin Setup
//Output
// Wheel Drive
int FWDML = 50;
int FWDMR = 49;
int RWDML = 52;
int RWDMR = 51;
//Conveyors
int Con1 = 53;
int Con2 = 48;
int Con3 = 47;
int Con4 = 46;
int Con5 = 45;
// Linear Actuator and Rack Pinion
struct BiMotor FrontLinearActuator = {44,43};
struct BiMotor RearLinearActuator = {42,41};
struct BiMotor RackPinionMotor = {40,39};
//Digger Motor
int DiggerMotor = 38;
// PWM Drive Pins
int rightPwmPin = 13;
int leftPwmPin = 12;
// Input
// Current sensor feedback
int CurrentDig = A6;
int CurrentCon = A7;
// LinearActuator feedback
int FLAPotFeedback = A8;
// Limit Switch
int LimitSwitchPin= 23;
// Encoders feedback
int RightEncoderA = 24;
int RightEncoderB = 25;
int LeftEncoderA = 32;
int LeftEncoderB = 33;
int RackEncoderA = 26;
int RackEncoderB = 31;

// Global Variables
//Wheel Drive PID range
//Define the aggressive and conservative Tuning Parameters
int driveRPM = 56;
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Current Arrays
const int currentSensorReadCount= 20;
float sensitivity = 0.04;
int count = 0;
//Digger Relavant Data
double diggerCurrent[currentSensorReadCount];// the index of the current reading
double totalDigger = 0;           // the running total
double avgDigger = 0;             // the average
double currentValDigger = 0;
//Dumper Relavant Data
double dumperCurrent[currentSensorReadCount];
double totalDumper = 0;           // the running total
double avgDumper = 0;             // the average
double currentValDumper = 0;
//Actuator Variable Setup
//Drive Channels
Servo rightDrive;
Servo leftDrive;
//Drive Speeds
double speedRight = 0;
double speedLeft = 0;
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
char currentData[5];
//Robot Setup
void startUpProcedure(){
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
	//startUpProcedure();
	//Reserve 16 bytes for input
	inputString.reserve(16);
	//inputs are not set as arduino defaults pins to input
	// Setup Actuators
	//Drive Motors
	pinMode(FWDML, OUTPUT);
	pinMode(FWDMR, OUTPUT);
	pinMode(RWDML, OUTPUT);
	pinMode(RWDMR, OUTPUT);
	rightDrive.attach(rightPwmPin);
	rightDrive.writeMicroseconds(1500);
	leftDrive.attach(leftPwmPin);
	leftDrive.writeMicroseconds(1500);
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
	//attachInterrupt(digitalPinToInterrupt(LimitSwitchPin), stopRack, HIGH);
	//prepDigger();
	for (int thisReading = 0; thisReading < currentSensorReadCount; thisReading++){
		dumperCurrent[thisReading] = 0;
		diggerCurrent[thisReading] = 0;
	}
}
void loop() {
	//Collect Data
	//4.77 in
	//Collect Current Sensors every 50ms
	if(millis()%50 == 0){
		//Sense Data Points
		senseDumperCurrent();
		senseDiggerCurrent();
		//Report Data Points
		//reportDumpCurrent(String(senseDumperCurrent(),2));
		//reportDiggCurrent(String(senseDiggerCurrent(),2));
		count = count + 1;
		if (count >= currentSensorReadCount){
			count = 0;
		}
	}
	//Do left then right PI correction on motors based on Encoders
	//Prep Report Quad Encoders
	//Motor PID Loop
	if(millis()%25 == 0){
	}
	//Left Pi Correct
	//Right Pi Correct
	//Report Data every 1 sec
		/*At the Linux Server level it will report every 10 sec and log
		The Client User can accept and log feedback*/
}
//End Of Main
//Serial RX
//Parser
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    inputString = Serial.readStringUntil('\n');
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
    Serial.println("Output:"+inputString);
    inputString = "";
  }
}
void reportDiggCurrent(String data){
  Serial.println("S+"+data);
}
void reportDumpCurrent(String data){
  Serial.println("S-"+data);
}
//Current Sensor Collect
double senseDumperCurrent(){
  //Digger Relavant Data
  totalDumper = totalDumper - dumperCurrent[count];
  dumperCurrent[count] = analogRead(CurrentCon);
  dumperCurrent[count] = (dumperCurrent[count]-512)*5/1024/sensitivity-sensitivity;
  totalDumper = totalDumper + dumperCurrent[count];
  avgDumper = totalDumper/currentSensorReadCount;
  currentValDumper = avgDumper;
  return currentValDumper;
}
double senseDiggerCurrent(){
  //Digger Relavant Data
  totalDigger = totalDigger - diggerCurrent[count];
  diggerCurrent[count] = analogRead(CurrentDig);
  diggerCurrent[count] = (diggerCurrent[count]-512)*5/1024/sensitivity-sensitivity;
  totalDigger = totalDigger + diggerCurrent[count];
  avgDigger = totalDigger/currentSensorReadCount;
  currentValDumper = avgDigger;
  return currentValDumper;
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
			allWheelDriveMode();
			break;
		case 'F':
      frontWheelDriveMode();
			break;
		case 'R':
			rearWheelDriveMode();
			break;
		default :
			Serial.println("Failure:Invalid Drive Mode");
		break;
	}
}
void handleCommandH(String inputString){
	haltRobot();
}
void handleCommandN(String inputString){
	resetRobot();
}
void handleCommandP(String inputString){
	double targetAngle = inputString.substring(1).toInt();
	angleDigger(targetAngle);
}
void handleCommandD(String inputString){
	double targetDistance = inputString.substring(1).toInt();
	positionDigger(targetDistance);
}
void handleCommandL(String inputString){
	switch(inputString.charAt(1)){
	    case '+':
			  biDirectMotorForward(RearLinearActuator);
    	break;
    	case '-':
        biDirectMotorReverse(RearLinearActuator);
    	break;
      case '^':
        biDirectMotorOff(RearLinearActuator);
    	default:
        biDirectMotorOff(RearLinearActuator);
    		Serial.println("Failure:Invalid Mode");
   	break;
	}
}
void handleCommandM(String inputString){
	double motorPowerRatio = inputString.substring(3).toInt()/100.0;
	//Checks the direction	
	if(inputString.charAt(2) == '-'){
		motorPowerRatio *= -1;
	}
	//checks if left or right motor
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
			startConveyorsAndDigger();
		break;
		case '-':
			stopConveyorsAndDigger();
		break;
		default:
			//Assume that charAt(1) is a #
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
					Serial.println("Failure:Invalid Conveyor/Digger Motor #");
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
					Serial.println("Failure:Invalid Conveyor/Digger Motor #");
				break;
			}
		}
	}
}

//Digger functionality
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
void angleDigger(double goalAngle){
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
void positionDigger(double goalLength){
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
	analogWrite(rightPwmPin,pwmValue);
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
	analogWrite(leftPwmPin,pwmValue);
}
// Linear Actuator & Rack Pinion Control Functions
void biDirectMotorForward(struct BiMotor motor){
	digitalWrite(motor.sideA, HIGH);
	digitalWrite(motor.sideB, LOW);
}
void biDirectMotorReverse(struct BiMotor motor){
	digitalWrite(motor.sideA, LOW);
	digitalWrite(motor.sideB, HIGH);
}
void biDirectMotorOff(BiMotor motor){
	digitalWrite(motor.sideA, HIGH);
	digitalWrite(motor.sideB, HIGH);
}
// Motor Control Functions
void turnMotorOff(int pinNumber){
	digitalWrite(pinNumber, LOW);
}
void turnMotorOn(int pinNumber){
	digitalWrite(pinNumber, HIGH);
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
