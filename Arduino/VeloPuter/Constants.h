#include <Arduino.h>
#include <U8glib.h>

#define STRADA   // Configuration management: STRADA QUILTJE and QUATRO 

/* Arduino Micro pin configuration/ capabilities
 *
 *  See also: https://www.arduino.cc/en/Main/ArduinoBoardMicro

20 Digital input/outputs
Intrupts: on 5 lines.
PWM: on 7 lines
Analog in: 12 lines

 0          INT     (Serial RX)
 1          INT     (Serial TX)
 2          INT     (TWI SDA)
 3      PWM INT     (TWI SCL)
 4/A6           ANA
 5      PWM
 6/A7   PWM     ANA
 7          INT
 8/A8           ANA
 9/A9   PWM     ANA     // Note that Timer 1 does break the pwm in this pin
10/A10  PWM     ANA     // Note that Timer 1 does break the pwm in this pin
11      PWM
12/A11          ANA
13      PWM
A0              ANA
A1              ANA
A2              ANA
A3              ANA
A4              ANA
A5              ANA

*
*
*/

const byte switchCadPin =       0;      // NOTE: 0 and 1 are the RX and TX pins.
const byte switchSpdPin =       1;      // NOTE: 0 and 1 are the RX and TX pins.
const byte OLED_SDA =           2;      // used for the oled display through u8glib
const byte OLED_SCL =           3;      // used for the oled display through u8glib
const byte UNUSED1 =            4;
const byte ledHeadPin =         5;
const byte ledAuxPin =          6;
const byte switchBrakePin =     7;      // 7 had an interupt so use it for the brake.
const byte UNUSED2 =            8;
const byte ledRightPin =        9;
const byte ledLeftPin =         10;
const byte ledRearPin =         11;
const byte UNUSED3 =            12;
const byte speakerPin =         13;
const byte voltagePin =         A0;

#if defined(STRADA) || defined(QUATRO)
const byte switchHeadUpPin =    A2; // switch up and down for the STRADA to sync all head light switches.
const byte switchHeadDownPin =  A1;
const byte switchRightPin =     A3;
const byte switchLeftPin =      A4;
const byte UNUSED4 =            A5;
#elif defined(QUILTJE)
const byte switchRightPin =     A1;
const byte switchLeftPin =      A2;
const byte switchHeadDownPin =  A3;
const byte switchHeadUpPin =    A4;     // switch up and down for the STRADA to sync all head light switches.
#endif

/*
   Other constants which are used throughout the programm:

*/
const long tSleep_ms = 300000;              // 3 minutes 60000 = 1 min 15000 = 15 sec 600000 = 10 min 300000 = 5 min
const long tSleepNoCadSpd_ms = 1800000;     // Half an hour
const int  tPeriodBlink_ms = 333;           // 1.5 Hz
const byte numTimesToBlink = 4;             // 7 times high, 6 times low, = 13 = 4.3 s
//
#if defined(STRADA) 
const int  wheelCircumvention_mm = 1590;    // 406-50 wheel
#elif defined(QUILTJE)
const int  wheelCircumvention_mm = 1450;    // 406-28 wheel
#elif defined(QUATRO)
const int  wheelCircumvention_mm = 1526;    // 406-28 wheel
#endif

const byte speakerVolume = 50;
const byte setOledIntensity = 255;



//const unsigned long tSoftwareTimerInterrupt_us = 10000; // run the timer at 100 Hz.
const unsigned long tSoftwareTimerInterrupt_us = 40000; // run the timer at 25 Hz.

/*
  Defaullt intensiteiten
*/
const int leftLedOffIntensity = 0; // Note that currently pwm is disabled for the blinker pins. See of this has anything to do with the failing display/.
const int leftLedLowIntensity = 255;
const int leftLedMediumIntensity = 255;
const int leftLedHighIntensity = 255;
const int leftLedMaxIntensity = 255;

const int rightLedOffIntensity = 0;
const int rightLedLowIntensity = 255;
const int rightLedMediumIntensity = 255;
const int rightLedHighIntensity = 255;
const int rightLedMaxIntensity = 255;

const int rearLedOffIntensity = 8;
const int rearLedLowIntensity = 32;
const int rearLedMediumIntensity = 64;
const int rearLedHighIntensity = 64; // Note that this stops the up/down!
const int rearLedMaxIntensity = 255;

#if  defined(STRADA) 
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 16;
const int headLedMediumIntensity = 96;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; //uas is just an other headlinght
const int auxLedLowIntensity = 16;
const int auxLedMediumIntensity = 96;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;

#elif defined(QUILTJE)
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 32;
const int headLedMediumIntensity = 128;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; // uax is the floodlight
const int auxLedLowIntensity = 16;
const int auxLedMediumIntensity = 32;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;

#elif defined(QUATRO)
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 16;
const int headLedMediumIntensity = 96;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; // aux is the brakelight
const int auxLedLowIntensity = 255;
const int auxLedMediumIntensity = 255;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;
#endif
