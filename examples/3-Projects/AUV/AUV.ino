#include <Servo.h>
#include <MINDS-i-Common.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects an ESC plugged into pin 4
/ A servo plugged into pin 5
/ and ping sensors in pins 9, 10, and 11
/ optionally, IR sensors in pins 12 and 13
/***************************************************/

namespace minds_i_sensors = minds_i_common::sensors;

const bool IR_ENABLED    = false;
const int  CENTER        = 90;
const int  TURN          = 45;
const int  FWDSPEED      = 115;
const int  REVSPEED      = 70;
const int  BACKUP_TIME   = 2000;
const int  SIDE_RANGE    = 4000;
const int  HAZARD_DIST[] = {   800,  3000,  800};
                          //  left,center,right

Servo drive, frontsteer, backsteer;

void setup() {
  pinMode(13, INPUT);
  pinMode(12, INPUT);

  drive.attach(4);
  frontsteer.attach(5);
  backsteer.attach(6);

  steer(CENTER);
  drive.write(90);
  delay(2000);
}

//define a steer command that sets front and back servos
void steer(int out) {
  frontsteer.write(out);
  backsteer.write(180 - out);
}

void loop() {
  int right = minds_i_sensors::getPing(9);
  delay(10);
  int left  = minds_i_sensors::getPing(11);
  delay(10);
  int front = minds_i_sensors::getPing(10);

  if (	left  < HAZARD_DIST[0] ||
        front < HAZARD_DIST[1] ||
        right < HAZARD_DIST[2]   ) {
    //coast down for two seconds
    drive.write(90);
    delay(2000);

    //turn in the most favorable direction
    if (left > right) {
      steer(CENTER + TURN);
    } else {
      steer(CENTER - TURN);
    }

    //prime reverse
    drive.write(85);
    delay(50);
    drive.write(90);
    delay(50);

    //back up for BACKUP_TIME milliseconds
    drive.write(REVSPEED);
    uint32_t endTime = millis() + BACKUP_TIME;
    while (millis() < endTime) {
      //leave the loop if backup sensors see a wall
      if ( IR_ENABLED && (!digitalRead(13) | !digitalRead(12)) ) break;
    }

    //coast to a stop
    drive.write(90);
    steer(CENTER);
    delay(1000);
  }
  else {
    left  = constrain(left , 0, SIDE_RANGE);
    right = constrain(right, 0, SIDE_RANGE);

    //map the difference in lAve and rAve from +/- SIDE_RANGE to +/- TURN
    long steerValue = map( (left - right),
                           -SIDE_RANGE, SIDE_RANGE,
                           -TURN,       TURN);

    steer(CENTER - steerValue);
    drive.write(FWDSPEED);
  }
}
