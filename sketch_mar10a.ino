 // Include iBusBM Library
#include <IBusBM.h>


//dzojstik
#define RIGHT_JOYSTICK_LEFT_RIGHT_CHANNEL 0
#define RIGHT_JOYSTICK_UP_DOWN_CHANNEL 1

// motor driver 
#define R_EN 31 //zelena
#define L_EN 30 //narandzasta
#define LPWM 8 //crna
#define RPWM 9 //siva
 
// Create iBus Object
IBusBM ibus;
 
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

int leftSpeed = 0;
int rightSpeed = 0;

void motorPowerCalc(int leftRight, int upDown) {

 // Calculate base speed based on up/down joystick
  int baseSpeed = upDown;

  // Calculate adjustment for turning
  int turnAdjustment = leftRight;

  // Adjust speeds for turning
  if(baseSpeed > 0) {
    leftSpeed = baseSpeed + (turnAdjustment);
    rightSpeed = baseSpeed - (turnAdjustment);
  }else if(baseSpeed < 0) {
    leftSpeed = baseSpeed - (turnAdjustment);
    rightSpeed = baseSpeed + (turnAdjustment);
  }else {
    leftSpeed = 0;
    rightSpeed = 0;
  }

  // Ensure motor speeds are within range (-100 to 100)
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
}

int right_joystick_left_right_value = 0;
int right_joystick_up_down_value = 0;


 
void setup() {
  // Start serial monitor
  Serial.begin(9600);
 
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  // Attach iBus object to serial port
  ibus.begin(Serial1);

  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);
}
 
void loop() {

  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

 
  right_joystick_up_down_value = readChannel(RIGHT_JOYSTICK_UP_DOWN_CHANNEL, -255, 255, 0);
  right_joystick_left_right_value = readChannel(RIGHT_JOYSTICK_LEFT_RIGHT_CHANNEL, -255, 255, 0);

  if(right_joystick_left_right_value >= -5 && right_joystick_left_right_value <= 5) {
    right_joystick_left_right_value = 0;
  }

  if(right_joystick_up_down_value >= -5 && right_joystick_up_down_value <= 5) {
    right_joystick_up_down_value = 0;
  }

  motorPowerCalc(right_joystick_left_right_value, right_joystick_up_down_value);
 
  Serial.print("left motor speed ");
  Serial.print(leftSpeed);
  Serial.print(" | ");
  Serial.print("right motor speed ");
  Serial.print(rightSpeed);
  Serial.print(" | ");
  Serial.println();

  if(leftSpeed > 0) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, abs(leftSpeed));
    Serial.print("ide pravo ");
    Serial.print(abs(leftSpeed));
    Serial.println();
  }else if(leftSpeed < 0) {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, abs(leftSpeed));
    Serial.print("ide nazad ");
    Serial.print(abs(leftSpeed));
    Serial.println();
  }else {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
    Serial.print("stoji");
    Serial.println();
  }
 delay(10);

  // analogWrite(LPWM, 0);
  // analogWrite(RPWM, 30);
  // delay(1000);
  // analogWrite(LPWM, 0);
  // analogWrite(RPWM, 150);
  // delay(2000);
  // analogWrite(LPWM, 0);
  // analogWrite(RPWM, 0);
  // delay(1000);

  // analogWrite(LPWM, 50);
  // analogWrite(RPWM, 0);
  // delay(1000);

}