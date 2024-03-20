 // Include iBusBM Library
#include <IBusBM.h>


//dzojstik
#define RIGHT_JOYSTICK_LEFT_RIGHT_CHANNEL 0
#define RIGHT_JOYSTICK_UP_DOWN_CHANNEL 1

// right driver 
#define right_motor_L_EN 2 // zuta l_en
#define right_motor_R_EN  3 // bijela r_en
#define right_motor_RPWM 4 // ljubicasta rpwm
#define right_motor_LPWM 5 // narandzasta lpwm

// left motor
#define left_motor_LPWM 8 //crna
#define left_motor_RPWM 9 //siva
#define left_motor_L_EN 10 //narandzasta
#define left_motor_R_EN 11 //zelena

#define THRESHOLD 5
 
// Create iBus Object
IBusBM ibus;

int leftMotorSpeed = 0;
int rightMotorSpeed = 0;


int right_joystick_left_right_value = 0;
int right_joystick_up_down_value = 0;


void setup() {
  // Start serial monitor
  Serial.begin(9600);
 
  pinMode(left_motor_R_EN, OUTPUT);
  pinMode(left_motor_L_EN, OUTPUT);
  pinMode(left_motor_RPWM, OUTPUT);
  pinMode(left_motor_LPWM, OUTPUT);

  pinMode(right_motor_R_EN, OUTPUT);
  pinMode(right_motor_L_EN, OUTPUT);
  pinMode(right_motor_RPWM, OUTPUT);
  pinMode(right_motor_LPWM, OUTPUT);

  // Attach iBus object to serial port
  ibus.begin(Serial1);

  // ensure left and right motor stopped
  analogWrite(left_motor_RPWM, 0);
  analogWrite(left_motor_LPWM, 0);
  analogWrite(right_motor_RPWM, 0);
  analogWrite(right_motor_LPWM, 0);
}
 
void loop() {
  digitalWrite(left_motor_R_EN, HIGH);
  digitalWrite(left_motor_L_EN, HIGH);
  digitalWrite(right_motor_R_EN, HIGH);
  digitalWrite(right_motor_L_EN, HIGH);

  right_joystick_up_down_value = readChannel(RIGHT_JOYSTICK_UP_DOWN_CHANNEL, -255, 255, 0);
  right_joystick_left_right_value = readChannel(RIGHT_JOYSTICK_LEFT_RIGHT_CHANNEL, -255, 255, 0);

  if(abs(right_joystick_left_right_value) <= THRESHOLD) {
    right_joystick_left_right_value = 0;
  }

  if(abs(right_joystick_up_down_value) <= THRESHOLD) {
    right_joystick_up_down_value = 0;
  }

  motorsPowerCalc(right_joystick_left_right_value, right_joystick_up_down_value);

  debugPrintValues();

  setMotorSpeed(leftMotorSpeed, left_motor_LPWM, left_motor_RPWM);
  setMotorSpeed(rightMotorSpeed, right_motor_LPWM, right_motor_RPWM);
}


void setMotorSpeed(int speed, int LPWM, int RPWM) {
  if(speed > 0) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, abs(speed));
  } else if(speed < 0) {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, abs(speed));
  } else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
  }
}


void motorsPowerCalc(int leftRight, int upDown) {
  // Adjust speeds for turning
  if(upDown > 0) {
    leftMotorSpeed = upDown + (leftRight);
    rightMotorSpeed = upDown - (leftRight);
  }else if(upDown < 0) {
    leftMotorSpeed = upDown - (leftRight);
    rightMotorSpeed = upDown + (leftRight);
  }else {
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
  }

  // Ensure motor speeds are within range (-255 to 255)
  leftMotorSpeed = constrain(leftMotorSpeed, -255, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 255) return defaultValue;
  if(ch >= -5 && ch <= 5) {
    ch = 0;
  }
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
 
// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 1, 100, intDefaultValue);
  return (ch > 50);
}

void debugPrintValues() {
  Serial.print("left motor speed ");
  Serial.print(leftMotorSpeed);
  Serial.print(" | ");
  Serial.print("right motor speed ");
  Serial.print(rightMotorSpeed);
  Serial.print(" | ");
  Serial.println();
}