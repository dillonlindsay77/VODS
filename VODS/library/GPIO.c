/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: GPIO.c
 * First Created: 2018-01-28
 * Last Modified: 2018-01-28
 * Author: Dillon Lindsay
 *
 * Description:
 * This file contains all of the definitions used for any GPIO attachments
 * to the Raspberry Pi in use with the Vertical Object Detection System.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <wiringPi.h>

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables to change the status LEDs.
 */
int REDLEDPIN = 22;
int YELLOWLEDPIN = 21;
int GREENLEDPIN = 30;
int OFF = 0;
int RED = 1;
int YELLOW = 2;
int GREEN = 3;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables for the sixteen LEDs pins.
 */
int L01 = 15;
int L02 = 16;
int L03 = 1;
int L04 = 4;
int L05 = 5;
int L06 = 6;
int L07 = 10;
int L08 = 11;
int L09 = 31;
int L10 = 26;
int L11 = 27;
int L12 = 28;
int L13 = 29;
int L14 = 25;
int L15 = 24;
int L16 = 23;
int ledArray[] = {L01,L02,L03,L04,L05,L06,L07,L08,L09,L10,L11,L12,L13,L14,L15,L16};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to setup GPIO as outupts.
 */
void setupOutputs(void)
{
    for(int i = 0; i < ARRAY_LEN(ledArray); i++) {
        pinMode(ledArray[i], OUTPUT);
    }
    pinMode(REDLEDPIN, OUTPUT);
    pinMode(YELLOWLEDPIN, OUTPUT);
    pinMode(GREENLEDPIN, OUTPUT);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to flash the provided pin.
 */
void flash(int pin)
{
    digitalWrite(pin, HIGH);
    delay(62.5);
    digitalWrite(pin, LOW);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to create a pulse train on the 16 test LEDs.
 */
void train(float aDelay)
{
    for(int i = 0; i < ARRAY_LEN(ledArray); i++) {
        digitalWrite(ledArray[i], HIGH);
        delay(aDelay);
        digitalWrite(ledArray[i], LOW);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to set the status LED to OFF, RED, GREEN, or BLUE.
 */
void setLED(int color)
{
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(YELLOWLEDPIN, LOW);
    digitalWrite(GREENLEDPIN, LOW);

    if(color == OFF) {
        digitalWrite(REDLEDPIN, LOW);
        digitalWrite(YELLOWLEDPIN, LOW);
        digitalWrite(GREENLEDPIN, LOW);
    } else if(color == RED) {
        digitalWrite(REDLEDPIN, HIGH);
    } else if(color == YELLOW) {
        digitalWrite(YELLOWLEDPIN, HIGH);
    } else if(color == GREEN) {
        digitalWrite(GREENLEDPIN, HIGH);
    } else {
        digitalWrite(REDLEDPIN, HIGH);
        digitalWrite(YELLOWLEDPIN, HIGH);
        digitalWrite(GREENLEDPIN, HIGH);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to set the color of the status LED by passing in OFF, RED, GREEN, or BLUE.
 */
void setProtoboardLEDs(float aArray[])
{
  // Tolerance is height of object.
  float tolerance = 0.07;

  for(int i = 0; i < ARRAY_LEN(ledArray); i++) {
    digitalWrite(ledArray[i], LOW);
  }

  for(int i = 0; i < ARRAY_LEN(ledArray); i++) {
    if(aArray[i] >= tolerance) {
      digitalWrite(ledArray[15-i], HIGH);
    }
  }
}

// End of file GPIO.c
