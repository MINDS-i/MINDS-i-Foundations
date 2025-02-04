#include <MINDS-i-Common.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example will drive with radio control until
/ ping sensors detect danger, in which case they will
/ override the radio input and turn the vehicle
/ away from the obstacle
/***************************************************/

namespace minds_i_comms = minds_i_common::comms;
namespace minds_i_sensors = minds_i_common::sensors;

const uint8_t PingPin[] = {7, 8, 9, 10, 11}; // left to right
const uint8_t ServoPin[] = {4, 5, 6};        // drive, steer, backsteer
const uint8_t RadioPin[] = {2, 3};           // drive, steer
const uint16_t warn[] = {1000, 1400, 2500, 1400, 1000};

const int STOP_TIME = 1500;               // time to coast to stop
const int DANGER_TIME = STOP_TIME + 1000; // time to backup after last sighting

const double pAngle[5] = {79.27l, 36.83l, 0.l, -36.83l, -79.27l};
const double ISTR = 1.l;
const double PSTR = 1350.l;

Servo servo[3]; // drive, steer, backSteer
uint8_t pIter;
uint16_t ping[5] = {20000, 20000, 20000, 20000, 20000};
int outputAngle, backDir;
uint32_t oTime, nTime, uTime;

void setup() {
    for (int i = 0; i < 3; i++)
        servo[i].attach(ServoPin[i]);
    output(90, 90);
    delay(2000);
    Serial.begin(9600);
    uTime = millis();
    minds_i_comms::getRadio(RadioPin[0]);
}

void loop() {
    if (uTime <= millis()) {
        uTime += 100;
        checkPing();
        navigate();
    }
}

void checkPing() {
    ping[pIter] = minds_i_sensors::getPing(PingPin[pIter]);
    pIter++;
    pIter = pIter % 5;
    if (ping[pIter] < warn[pIter])
        oTime = millis();
}

void navigate() {
    if (oTime != 0) {
        if (nTime == 0) {
            output(90, 90);
            nTime = millis();
            backDir = (ping[0] < ping[4]) ? 65 : 115;
        } else if (nTime + STOP_TIME < millis()) {
            output(75, backDir);
        }

        if (oTime + DANGER_TIME < millis()) {
            nTime = 0;
            oTime = 0;
        }
    } else {
        double inputAngle = minds_i_comms::getRadio(RadioPin[1]) - 90;
        double x, y;
        x = ISTR * cos(toRad(inputAngle));
        y = ISTR * sin(toRad(inputAngle));

        for (int i = 0; i < 5; i++)
            checkPing();
        for (int i = 0; i < 5; i++) {
            double tmp = ping[i] / PSTR;
            tmp *= tmp;
            x += cos(toRad(pAngle[i])) / tmp;
            y += sin(toRad(pAngle[i])) / tmp;
        }

        outputAngle = toDeg(atan2(y, x)) + 90;
        outputAngle = constrain(outputAngle, 45, 135);
        output(minds_i_comms::getRadio(RadioPin[0]), outputAngle);
    }
}

void output(uint8_t drive, uint8_t steer) {
    servo[0].write(drive);
    servo[1].write(steer);
    servo[2].write(180 - steer);
}

double toRad(double degrees) {
    degrees /= 180.l;
    degrees *= PI;
    return degrees;
}

double toDeg(double radians) {
    radians /= PI;
    radians *= 180.l;
    return radians;
}
