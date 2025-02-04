#include "SPI.h"
#include "Wire.h"

#include "Servo.h"
#include <MINDS-i-Common.h>
#include <EEPROM.h>
#include "pins_arduino.h"

/*******************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ Capable of testing the standard 4x4 layout for errors
/ Or, Use to read any number of:
/ analog Sensors
/ digital Sensors
/ QTI Sensors
/ Ping Sensors
/ And reading/clearing EEPROM
/*******************************************************/

namespace minds_i_comms = minds_i_common::comms;
namespace minds_i_sensors = minds_i_common::sensors;

const int dQTIs[3] = {A0, A1, A2};
const int dPing[3] = {9 , 10, 11};
const int NUM_DIGITAL_ONLY = NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS;

Servo testServo;
bool pins[NUM_DIGITAL_PINS];
bool cont;
char tmp;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const int maxEEPROM = 4096;
#else
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__)
const int maxEEPROM = 512;
#else
const int maxEEPROM = 1024;
#endif
#endif

void menu();
void selfTest();
void readFromPins(String string, int (*read)(int) );
void loadEeprom();
void motorController();
void empty();
int analogIntRead(int pin);
int digitalIntRead(int pin);
int simpleRadioRead(int pin);
int getIntPing(int pin);
int getIntQTI(int pin);

void setup() {
  Serial.begin(9600);
  while(!Serial) /*nop*/; //wait for leonardo to connect to PC
  delay(500);
  menu();
  Serial.flush();
}

void loop() {
  if(!Serial) setup(); //reset for leonardos that have had their serial port closed
  switch (Serial.read()) {
    case 'a':
    case 'A':
      readFromPins("Analog Pins", analogIntRead);
      menu();
      break;
    case 'd':
    case 'D':
      readFromPins("Digital Pins", digitalIntRead);
      menu();
      break;
    case 'P':
    case 'p':
      readFromPins("Ping Sensors", getIntPing);
      menu();
      break;
    case 'Q':
    case 'q':
      readFromPins("QTI Sensors", getIntQTI);
      menu();
      break;
    case 'R':
    case 'r':
      readFromPins("Radio Pins", simpleRadioRead);
      menu();
      break;
    case 'E':
    case 'e':
      empty();
      loadEeprom();
      menu();
      break;
    case 'S':
    case 's':
      empty();
      selfTest();
      menu();
      break;
    case 'M':
    case 'm':
      empty();
      motorController();
      menu();
      break;
  }
}

void menu() {
  Serial.println(
    "\nEnter the character for what you want to do."
    "\n(S)elf test; (Q)TIs; (P)ings; (D)igitals;"
    " (A)nalogs; (E)EPROM; (M)otor; (R)adio");
}

void selfTest() {
  bool pass = true;
  Serial.println("\nAttempting to get a reading from each sensor\n");

  for (int i = 0; i < 3; i++) {
    if (minds_i_sensors::getPing(dPing[i]) == 0) {
      Serial.print("Ping #");
      Serial.print(i);
      Serial.print(" on pin ");
      Serial.print(dPing[i]);
      Serial.print(" is unresponsive");
      Serial.print("\n");
      pass = false;
    }
  }

  Serial.print("\n");

  for (int i = 0; i < 3; i++) {
    int tmp = minds_i_common::sensors::QTI(dQTIs[i]);
    if ( tmp == 10000 ) {
      Serial.print("QTI #");
      Serial.print(i);
      Serial.print(" on pin ");
      Serial.print(dQTIs[i]);
      Serial.print(" is unresponsive, check connection");
      Serial.print("\n");
      pass = false;
    } else if (tmp == 0) {
      Serial.print("QTI #");
      Serial.print(i);
      Serial.print(" on pin ");
      Serial.print(dQTIs[i]);
      Serial.print(" failed; check wire order;"
                   "B->Ground R->Sig(white), W->Pos");
      Serial.print("\n");
      pass = false;
    }
  }

  if (pass) Serial.println("\nTest Passed!\n");
  else Serial.println("\nTest Failed!\n");
}

void readFromPins(String string, int (*read)(int) ) {
  for (int i = 0; i < NUM_DIGITAL_PINS; i++) pins[i] = 0;
  Serial.print("\nenter the ");
  Serial.print(string);
  Serial.print(" pins you want to monitor");
  while (! Serial.available());

  int tmp;

  cont = true;
  while (cont) {
    while (Serial.available()) {
      if (Serial.peek() == 'q') cont = false;
      if (Serial.peek() == 'A') {
        Serial.read();
        tmp = Serial.parseInt();
        tmp += NUM_DIGITAL_ONLY;
      } else {
        tmp = Serial.parseInt();
      }
      if (tmp > 0 && tmp <= NUM_DIGITAL_PINS) pins[tmp] = !pins[tmp];
    }

    for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
      if (pins[i] == 1) {
        int tmp = read(i);
        Serial.print(" pin");
        if (i >= (NUM_DIGITAL_ONLY)) {
          Serial.print("A");
          Serial.print(i - (NUM_DIGITAL_ONLY));
        } else {
          Serial.print(i);
        }
        Serial.print(": ");
        Serial.print(tmp);
      }

    }
    Serial.print("\n");
  }
}

void loadEeprom() {
  bool sentNewLines = false;
  for (int i = 0; i < maxEEPROM; i++) {
    tmp = EEPROM.read(i);
    if (sentNewLines == false && (tmp == '\n' || tmp == '\r'))
      sentNewLines = true;
    Serial.print( tmp );
    if (sentNewLines == false && i % 55 == 0) Serial.print("\n");
  }

  Serial.println("\nWould you like to erase the data? y/n");

  cont = true;
  while (cont) {
    switch (Serial.read()) {
      case 'y':
      case 'Y':
        for (int i = 0; i < maxEEPROM; i++) {
          if (i % 128 == 0) Serial.print(".");
          EEPROM.write(i, 0x00);
        }
        Serial.println("\nDone erasing data");
        cont = false;
        break;
      case 'n':
      case 'N':
        cont = false;
        break;
    }
  }
}

void motorController() {
  Serial.println("Type pin number of ESC or Servo to "
                 "Arm or Center or any letter to Quit");
  while (true) {
    if (Serial.available()) {
      int tmp = Serial.parseInt();
      if (tmp == 0) break;
      Serial.print("Activating pin ");
      Serial.print(tmp);
      Serial.println(" for 2 seconds");
      testServo.attach(tmp);
      testServo.write(90);
      for (int i = 0; i < 40; i++) {
        Serial.print('.');
        delay(50);
      }
      testServo.detach();
      Serial.println("\nDone. pin deactivated");
    }
  }
}

//function pointer helpers
int analogIntRead(int pin) {
  return analogRead(pin);
}
int digitalIntRead(int pin) {
  return digitalRead(pin);
}
int simpleRadioRead(int pin) {
  return minds_i_comms::getRadio(pin);
}
int getIntPing(int pin) {
  return minds_i_common::sensors::getPing(pin);
}
int getIntQTI(int pin) {
  return minds_i_common::sensors::QTI(pin);
}

void empty() {
  delay(10);
  while (Serial.available()) Serial.read();
}

