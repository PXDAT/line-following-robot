#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int bluetooth_signal = 0;

//Sensor pins
const int leftSensor = 5;
const int middleSensor = 7;
const int rightSensor = 9;

const int initSpeed = 800;


//PID Variables
int error = 0;
int previous_error = 0;
float kP = 400;
float kI = 0;
float kD = 200;
float P, I, D;
float PID_value;

void goForward(int leftSpeed, int rightSpeed){
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, leftSpeed);
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(13, 0, leftSpeed);

  pwm.setPWM(8, 0, rightSpeed);
  pwm.setPWM(9, 0, 0);
  pwm.setPWM(10, 0, rightSpeed);
  pwm.setPWM(11, 0, 0);
}
void turnRight(int leftSpeed, int rightSpeed){
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, leftSpeed);
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(13, 0, leftSpeed);

  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, rightSpeed);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, rightSpeed);
}
void turnLeft(int leftSpeed, int rightSpeed){
  pwm.setPWM(14, 0, leftSpeed);
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(12, 0, leftSpeed);
  pwm.setPWM(13, 0, 0);

  pwm.setPWM(8, 0, rightSpeed);
  pwm.setPWM(9, 0, 0);
  pwm.setPWM(10, 0, rightSpeed);
  pwm.setPWM(11, 0, 0);
}
void goBackward(int leftSpeed, int rightSpeed){
  pwm.setPWM(14, 0, leftSpeed);
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(12, 0, leftSpeed);
  pwm.setPWM(13, 0, 0);

  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, rightSpeed);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, rightSpeed);
}
void stopBot(){
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, 0);
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(13, 0, 0);

  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, 0);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(11, 0, 0);
}
void calculatePID(){
  P = error;
  I = I + error;
  D = error-previous_error;
  PID_value = (kP*P) + (kI*I) + (kD*D);
  previous_error = error;
}
void motorPIDControl(){
  int leftSpeed = initSpeed + PID_value;
  int rightSpeed = initSpeed - PID_value;

  pwm.setPWM(14, 0, 0);
  pwm.setPWM(15, 0, leftSpeed);

  pwm.setPWM(8, 0, 0);
  pwm.setPWM(9, 0, rightSpeed);
}
void lineFollowing(){
  if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 0)) error = 0;
  else if((digitalRead(leftSensor) == 1) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 0)) error = 1;
  else if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 0) && (digitalRead(rightSensor) == 1)) error = -2;
  else if((digitalRead(leftSensor) == 0) && (digitalRead(middleSensor) == 1) && (digitalRead(rightSensor) == 1)) error = -1;
  else if((digitalRead(leftSensor) == 1) && (digitalRead(middleSensor) == 0) && (digitalRead(rightSensor) == 0)) error = 2;
  calculatePID();
  motorPIDControl();
}
void setup(){
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(60);
  Wire.setClock(400000);

  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(middleSensor, INPUT);
  Serial.println("Initialize MakerBot");
  Serial.begin(115200);

  delay(2500);
}
void loop(){
  Serial.print(digitalRead(leftSensor));
  Serial.print(" ");
  Serial.print(digitalRead(middleSensor));
  Serial.print(" ");
  Serial.print(digitalRead(rightSensor));
  Serial.print("\n");
  Serial.print("Current error: ");
  Serial.println(error);
  lineFollowing();
}