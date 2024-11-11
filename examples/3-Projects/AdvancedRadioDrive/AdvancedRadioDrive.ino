#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects an ESC plugged into pin 4
/ A servo plugged into pin 5
/ and a radio plugged into pins 2 and 3
/***************************************************/

namespace minds_i_comms = minds_i_common::comms;

Servo steer, drive;
float driveOut, steerOut;
float driveIn, steerIn;

void setup() {
  drive.attach(4);
  steer.attach(5);
}

void loop() {
  if (minds_i_comms::isRadioOn(2)) {
    radioControl();
  } else {
    radioOff();
  }
}

void radioControl() {
  //exponential running average of radio signals to reduce noise
  driveIn = (driveIn * .6) + (minds_i_comms::getRadio(2) * .4);
  steerIn = (steerIn * .6) + (minds_i_comms::getRadio(3) * .4);

  driveOut = driveIn - 90;
  steerOut = steerIn - 90;
  driveOut = driveOut * driveOut * driveOut / 8100; //cubic motor curve
  steerOut = steerOut * steerOut * steerOut / 8100; //8100 = 90^2
  driveOut += 90;
  steerOut += 90;

  //deadbands to hold steady at center
  if ((driveOut > 95 || driveOut < 85)) {
    drive.write(driveOut);
  } else {
    drive.write(90);
  }

  if (steerOut > 95 || steerOut < 85) {
    steer.write(steerOut);
  } else {
    steer.write(90);
  }
}

void radioOff() {
  drive.write(90);
  steer.write(90);
}
