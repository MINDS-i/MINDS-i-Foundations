#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects a radio plugged into pin 2
/***************************************************/

namespace minds_i_comms = minds_i_common::comms;

int val;

void setup() {
  //start a serial connection
  Serial.begin(9600);
}

void loop() {
  //read the value being sent on pin 2
  val = minds_i_comms::getRadio(2);

  //transmit over the serial connection
  Serial.println(val);
}
