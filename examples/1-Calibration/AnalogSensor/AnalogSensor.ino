#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects a sensor to be plugged in to A0
/***************************************************/

int val;

void setup() {
  //start a serial connection
  Serial.begin(9600);
}

void loop() {
  //read in the value on analog pin 0
  val = analogRead(A0);
  //send a string or value on the serial connection
  Serial.println(val);
}
