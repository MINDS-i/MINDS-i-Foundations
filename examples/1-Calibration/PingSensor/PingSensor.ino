#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects a ping sensor plugged into pin 10
/ Or, Swap out the comments below to read three sensors
/ on pins 9,10, and 11
/***************************************************/

namespace minds_i_sensors = minds_i_common::sensors;

void setup() {
  //start a serial connection
  Serial.begin(9600);
}

void loop() {
  //send the value read by the ping sensor out serial
  Serial.println(minds_i_sensors::getPing(10));

  //optional code for displaying three QTI sensors, make sure the previous code
  //is commented out when using the code below.
  /*
  	Serial.print(minds_i_sensors::getPing(9));
  	Serial.print("\t"); //insert some tabs
  	Serial.print(minds_i_sensors::getPing(10));
  	Serial.print("\t");
  	Serial.println(minds_i_sensors::getPing(11));
  */

  // wait for potential echos to die down
  delay(50);
}
