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

float X, Y, Z;

void setup() {
  //start a serial connection
  Serial.begin(9600);
}

void loop() {
  //read in the value on analog pin 0
  X = analogRead(A0);
  Y = analogRead(A1);
  Z = analogRead(A2);

  //send title and then value over serial
  Serial.print("X:");
  Serial.print(X);
  Serial.print("\tY:"); // \t inserts a "tab"
  Serial.print(Y);
  Serial.print("\tZ:");
  Serial.print(Z);

  //shift down a line
  Serial.println();
}
