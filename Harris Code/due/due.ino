#include <Servo.h>

///////////////////////////////////////////////////////////////////////////////
// constants
///////////////////////////////////////////////////////////////////////////////

const int N_RELAYS_PER_BOARD = 8;
const int RBA[N_RELAYS_PER_BOARD] = {
  40, 41, 42, 43, 44, 45, 46, 47
};

const int FRR = 44;
const int FLR = 43;
const int BRR = 42;
const int BLR = 41;

const int PWM_LC = 6;
const int PWM_RC = 7;

const int SERIAL_BUFF_SIZE = 16;

const int COMMAND_SIZE = 3;

const char SERIAL_END = 0x00;

///////////////////////////////////////////////////////////////////////////////
// variable declarations
///////////////////////////////////////////////////////////////////////////////

char serialBuff[SERIAL_BUFF_SIZE] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int serialIndex = 0;
Servo leftChannel, rightChannel;

///////////////////////////////////////////////////////////////////////////////
// function prototypes
///////////////////////////////////////////////////////////////////////////////

void relay_on(int relay);
void relay_off(int relay);

void drive_mode_all(void);
void drive_mode_front(void);
void drive_mode_back(void);

void parse_command(void);

///////////////////////////////////////////////////////////////////////////////
// arduino functions
///////////////////////////////////////////////////////////////////////////////

void setup() {
  for (int i = 0; i < N_RELAYS_PER_BOARD; ++i) {
    pinMode(RBA[i], OUTPUT);
    relay_on(RBA[i]);
  }

  leftChannel.attach(PWM_LC);
  rightChannel.attach(PWM_RC);

  leftChannel.writeMicroseconds(1500);
  rightChannel.writeMicroseconds(1500);

  Serial.begin(9600);
}

void loop() {
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();

      // we have received a terminating character
      if (c == SERIAL_END) break;

      // we have read more than SERIAL_BUFF_SIZE characters without a terminating character,
      // something has gone horribly wrong
      if (serialIndex < SERIAL_BUFF_SIZE) {
        serialBuff[serialIndex] = c;
        serialIndex++; 
      }
    }
  }

  parse_command();

  serialIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////
// function definitions
///////////////////////////////////////////////////////////////////////////////

// turns the relay on allowing current to flow through it
void relay_on(int relay) {
  digitalWrite(relay, HIGH);
}

// turns the relay off preventing current from flowing through it
void relay_off(int relay) {
  digitalWrite(relay, LOW);
}

void drive_mode_all(void) {
  relay_on(FRR);
  relay_on(FLR);
  relay_on(BRR);
  relay_on(BLR);
}

void drive_mode_front(void) {
  relay_on(FRR);
  relay_on(FLR);
  relay_off(BRR);
  relay_off(BLR);
}

void drive_mode_back(void) {
  relay_off(FRR);
  relay_off(FLR);
  relay_on(BRR);
  relay_on(BLR);
}

void parse_command(void) {
  // we need exactly three characters for every serial communication
  if (serialIndex != COMMAND_SIZE) {
    return;
  }
  
  char a, b, c;

  // the three command characters sent from the client
  a = serialBuff[0];
  b = serialBuff[1];
  c = serialBuff[2];

  // set channel
  if (a == 'c') {
    float val = (float)((byte)c) / 255.0f;
    val = (val * 1000.0f) + 1000.0f;
    int ms = (int)val;

    if (b == 'l') {
      leftChannel.writeMicroseconds(ms);
    } else if (b == 'r') {
      rightChannel.writeMicroseconds(ms);
    }
  // change drive mode
  } else if (a == 'm') {
    if (b == 'a') drive_mode_all();
    else if (b == 'f') drive_mode_front();
    else if (b == 'b') drive_mode_back();
  }
}

