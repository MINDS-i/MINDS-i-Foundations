#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example shows how to switch control between
/ two modes by determining if the radio input to
/ pin 3 is currently active. Note that not all radio
/ receivers turn off when the remote signal is lost,
/ in which case robot will never leave radioControl
/ mode
/***************************************************/

namespace minds_i_comms = minds_i_common::comms;

Servo steer, drive;

void setup() {
  drive.attach(4);
  steer.attach(5);

  drive.write(90);
  steer.write(90);

  // start interrupts on pin 3 so pulses will be captured before isRadioOn
  // is first called
  minds_i_comms::getRadio(3);

  delay(2000);
}

void loop() {
  if (minds_i_comms::isRadioOn(3)) {
    radioControl();
  } else {
    radioOff();
  }
}

void radioControl() {
}

void radioOff() {
}
