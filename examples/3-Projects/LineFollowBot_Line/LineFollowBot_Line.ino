#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects 2 servos in pins 4 and 5
/ and three QTI sensors in pins A0, A1, and A2
/***************************************************/

namespace minds_i_sensors = minds_i_common::sensors;

Servo left, right;

float Lout, Rout;
float Lset, Rset;

const double rxtime = 0.25;
const int neutral = 90;
const int leftCenter = 4;
const int rightCenter = 5;
const int outsideSpeed = 45;
const int insideSpeed = 12;
const int forwardSpeed = 35;

void setup() {
    Serial.begin(9600);

    left.attach(5);
    right.attach(4);

    left.write(neutral);
    right.write(neutral);

    Rout = neutral;
    Lout = neutral;
}

void loop() {
    bool left = minds_i_sensors::QTI(A0) > 120;
    bool middle = minds_i_sensors::QTI(A1) > 120;
    bool right = minds_i_sensors::QTI(A2) > 120;

    if (left) {
        Lset = neutral + insideSpeed;
        Rset = neutral + outsideSpeed;
    } else if (right) {
        Lset = neutral - outsideSpeed;
        Rset = neutral - insideSpeed;
    } else if (middle) {
        Lset = neutral - forwardSpeed;
        Rset = neutral + forwardSpeed;
    }

    output();
}

// all this is because the QTI Blocks for a variable amount of time
void output() {
    static uint32_t time = millis();

    double dT = (double(millis() - time) / 100);
    time = millis();

    double adj = pow(rxtime, dT);

    Rout = (Rout * adj) + (Rset * (1 - adj));
    Lout = (Lout * adj) + (Lset * (1 - adj));

    left.write(Lout + leftCenter);
    right.write(Rout + rightCenter);
}
