#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example shows how to switch control between
/ two modes based on a radio switch plugged in to
/ pin 7
/***************************************************/

namespace minds_i_comms = minds_i_common::comms;

Servo steer, drive;

void setup() {
  drive.attach(4);
  steer.attach(5);

  drive.write(90);
  steer.write(90);

  delay(2000);
}

void loop() {
  if (minds_i_comms::getRadio(7) > 90) {
    radioControl();
  } else {
    radioOff();
  }
}

void radioControl() {
}

void radioOff() {
}
