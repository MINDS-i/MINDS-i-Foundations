#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects an ESC plugged into pin 4
/***************************************************/

Servo motor;

void setup() {
    // set a pin for a servo/ESC to use
    motor.attach(4);

    // set the initial throttle
    motor.write(90);

    // wait for 2 seconds to arm the ESC
    delay(2000);
}

void loop() {}
