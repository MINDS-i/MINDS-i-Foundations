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
/ and a ping sensor in pin 10
/***************************************************/

namespace minds_i_sensors = minds_i_common::sensors;

Servo drive, steer;

void setup() {
    // set a pin for the ESC/steering servo to use
    drive.attach(4);
    steer.attach(5);

    // set the initial throttle/direction for the ESC/servo
    drive.write(90);
    steer.write(90);

    // delay 2 seconds for arming
    delay(2000);

    // turn the wheel
    steer.write(120);
}

void loop() {
    // if there is no room in front, stop
    if (minds_i_sensors::getPing(10) < 1500) {
        drive.write(90);
    } else {
        // otherwise go forward
        drive.write(100);
    }

    // wait for potential echos to die down
    delay(50);
}
