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
namespace minds_i_comms = minds_i_common::comms;

const bool IR_ENABLED    = false;
const int  CENTER        = 90;
const int  TURN          = 45;
const int  FWDSPEED      = 115;
const int  REVSPEED      = 70;
const int  BACKUP_TIME   = 2000;
const int  SIDE_RANGE    = 4000;
const int  PING_PINS[]   = {      7,      8,      9,     10,     11};
const int  HAZARD_DIST[] = {    650,    800,   3000,    800,    650};
const int                    WEST=0, LEFT=1,  FWD=2,RIGHT=3, EAST=4;

Servo drive, frontsteer, backsteer;
int ping[5];

void setup() {
  Serial.begin(9600);

  pinMode(13, INPUT);
  pinMode(12, INPUT);

  drive.attach(4);
  frontsteer.attach(5);
  backsteer.attach(6);

  // start interrupts on pin 3 so pulses will be captured before isRadioOn
  // is first called
  minds_i_comms::getRadio(3);

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
  //this will give manual control if a radio is plugged in
  if (minds_i_comms::isRadioOn(3)) {
    drive.write(minds_i_comms::getRadio(2));
    steer(minds_i_comms::getRadio(3));
  } else {
    autodrive();
  }
}

void autodrive() {
  bool danger = false;
  for (int i = 0; i < 5; i++) {
    ping[i] = minds_i_sensors::getPing(PING_PINS[i]);
    if (ping[i] < HAZARD_DIST[i]) danger = true;
    delay(10);
  }

  if (danger) {
    //coast down for two seconds
    drive.write(90);
    delay(2000);

    //turn in the most favorable direction
    if (ping[LEFT] > ping[RIGHT]) {
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
    //calculate average distance on each side
    long left  = min(ping[LEFT] , ping[WEST]);
    long right = min(ping[RIGHT], ping[EAST]);
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
