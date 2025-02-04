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
/ and a trio of QTI's in A0, A1, and A2
/
/ Proper QTI sensor wiring:
/                   ---------------------
/ Positive (Red)  --| W   .-.   |-|-| i |
/ Signal (White)  --| R  (   )  |-|-| T |
/ Ground (black)  --| B   '-'   |-|-| Q |
/                   ---------------------
/***************************************************/

namespace minds_i_sensors = minds_i_common::sensors;

Servo steer, drive;

const int threshold = 300;

void setup() {
    // set a pin for the ESC/steering servo to use
    drive.attach(4);
    steer.attach(5);

    // set the initial throttle/direction for the ESC/servo
    drive.write(90);
    steer.write(90);

    // delay 2 seconds for arming
    delay(2000);

    // start driving forward slowly
    drive.write(100);
}

void loop() {
    // first check if the left sensor is on the line, if not check the right sensor
    // and finally check if the center sensor says to go strait, otherwise
    // don't change the desired direction

    if (minds_i_sensors::QTI(A0) < threshold) {
        steer.write(45);
    } else if (minds_i_sensors::QTI(A2) < threshold) {
        steer.write(135);
    } else if (minds_i_sensors::QTI(A1) < threshold) {
        steer.write(90);
    }
}
