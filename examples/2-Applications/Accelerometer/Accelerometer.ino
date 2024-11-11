#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects your accelerometer to be wired
/ with X->A0, y->A1, z->A0
/***************************************************/

const int downReading = 700;
Servo drive, steer;
float Z;

void setup() {
  //set a pin for the ESC/steering servo to use
  drive.attach(4);
  steer.attach(5);

  //set the initial throttle/direction for the ESC/servo
  drive.write(90);
  steer.write(90);

  //delay 2 seconds for ESC arming
  delay(2000);
}

void loop() {
  //store the value read
  Z = analogRead(A2) / 1024;

  //if less than half of normal G's is read on the Z axis (up/down), stop
  if (Z < downReading) {
    drive.write(90);
  } else {
    drive.write(100);
  }
}
