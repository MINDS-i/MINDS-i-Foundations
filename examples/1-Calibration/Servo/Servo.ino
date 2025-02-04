#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects a servo plugged into pin 5
/***************************************************/
Servo myservo;

void setup() {
  //set a pin for a servo/ESC to use
  myservo.attach(5);

  //set the output to 90 degrees (center)
  myservo.write (90);
}

void loop() {
}


