typedef struct BiMotor{
  int sideA;
  int sideB;
};
// Linear Actuator and Rack Pinion
struct BiMotor FrontLinearActuator = {44,45};
struct BiMotor RearLinearActuator = {47,48};
struct BiMotor RackPinionMotor = {42,43};
int FLAPotFeedback = A0;
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
//Feedback Functions
double angleReadOut(int value){
  value -= 100;
  double reading = 1 - value/915.0;
  double s = reading * 6;
  s = s + 12;
  return 2 * atan((275 - (4*sqrt((-4 * pow(s, 4)) + (2965 * pow(s, 2)) - 342225))) / ((8 * pow(s, 2)) - 4765)) * (180 / 3.14159f);
}
void angleDigger(double goalAngle){
  double sensorValue = angleReadOut(analogRead(FLAPotFeedback));
  Serial.println(sensorValue);
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
    if(millis()%50 == 0){
      sensorValue = angleReadOut(analogRead(FLAPotFeedback));
    }
    Serial.println(sensorValue);
  }
  biDirectMotorOff(FrontLinearActuator);
  //print out to serial hit event and value at
}

void setup(){
  Serial.begin(9600);
  pinMode(FLAPotFeedback, INPUT);
  for(int i = 42; i < 49;i++){
    if(i != 46){
      pinMode(i, OUTPUT);
    }
  }
  biDirectMotorOff(FrontLinearActuator);
  biDirectMotorOff(RearLinearActuator);
  biDirectMotorOff(RackPinionMotor);
  //test each
  biDirectMotorForward(FrontLinearActuator);
  delay(1000);
  biDirectMotorReverse(FrontLinearActuator);
  delay(1000);
  biDirectMotorOff(FrontLinearActuator);
  biDirectMotorForward(RearLinearActuator);
  delay(1000);
  biDirectMotorReverse(RearLinearActuator);
  delay(1000);
  biDirectMotorOff(RearLinearActuator);
  biDirectMotorForward(RackPinionMotor);
  delay(1000);
  biDirectMotorReverse(RackPinionMotor);
  delay(1000);
  biDirectMotorOff(RackPinionMotor);
  /* If Pot is fixed
  angleDigger(60);
  delay(1000);
  angleDigger(45);
  delay(1000);
  angleDigger(30);
  delay(1000);
  angleDigger(15);
  delay(1000);
  */
}
void loop(){
}
