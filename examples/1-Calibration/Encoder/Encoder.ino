#include <Servo.h>
#include <MINDS-i-Common.h>
#include <MINDS-i-Common-Encoder.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects an encoder plugged into pins 2 and 3
/ If you get the order wrong, rolling backwards
/ would read as positive rpm and vice versa
/***************************************************/

float rpm;
float rev;
float mph;

const int encoderPins[2] = {2, 3};

//Since these conversions vary based on measurements of the robot,
//these must be calculated here
//(wheel Diameter*PI) * minutes per hour / (inches per mile * gear ratio)
float RPMtoMPH = ((5. *PI) * 60.) / (63360.*(37. / 13.));

void setup() {
  Serial.begin(9600);  //start a serial connection
  encoder::begin(encoderPins[0], encoderPins[1]);
}

void loop() {
  rpm = encoder::getRPM();
  rev = encoder::getRev();
  mph = rpm * RPMtoMPH;

  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print("\tMPH: ");
  Serial.print(mph);
  Serial.print("\tREV: ");
  Serial.print(rev);
  Serial.print("\n");
}
