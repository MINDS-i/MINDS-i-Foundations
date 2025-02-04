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

Servo drive, steer;
int driveSig, steerSig;

void setup() {
    // set a pin for the ESC/steering servo to use
    drive.attach(4);
    steer.attach(5);

    // set the initial throttle/direction for the ESC/servo
    drive.write(90);
    steer.write(90);

    // delay 2 seconds for arming
    delay(2000);
}

void loop() {
    // store the inbound radio value
    driveSig = minds_i_comms::getRadio(2);
    steerSig = minds_i_comms::getRadio(3);

    // set the ESC/servo to the inbound radio value
    drive.write(driveSig);
    steer.write(steerSig);
}
