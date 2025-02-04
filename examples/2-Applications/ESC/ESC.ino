#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects an ESC plugged into pin 4
/ It will also center a servo in pin 5 for convenience
/***************************************************/

Servo drive, steer;

void setup() {
  //set a pin for the ESC/steering servo to use
  drive.attach(4);
  steer.attach(5);

  //set the output for the ESC/servo
  drive.write(90);
  steer.write(90);

  //delay 2 seconds for arming
  delay(2000);
}

void loop() {
  //drive forward one second
  drive.write(100);
  delay(1000);

  //brake for 1 second
  drive.write(45);
  delay(1000);

  //disengage the brakes
  drive.write(90);
  delay(50);

  //drive backward one second
  drive.write(80);
  delay(1000);

  //coast for 1 second
  drive.write(90);
  delay(1000);
}
