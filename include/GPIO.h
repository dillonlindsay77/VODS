/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: GPIO.h
 * First Created: 2018-01-28
 * Last Modified: 2018-01-29
 * Author: Dillon Lindsay
 *
 * Description:
 * This file contains all of the definitions used for any GPIO attachments
 * to the Raspberry Pi in use with the Vertical Object Detection System.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables to change the status LEDs.
 */
extern int REDLEDPIN;
extern int YELLOWLEDPIN;
extern int GREENLEDPIN;
extern int OFF;
extern int RED;
extern int YELLOW;
extern int GREEN;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables for the sixteen LEDs pins.
 */
extern int L01;
extern int L02;
extern int L03;
extern int L04;
extern int L05;
extern int L06;
extern int L07;
extern int L08;
extern int L09;
extern int L10;
extern int L11;
extern int L12;
extern int L13;
extern int L14;
extern int L15;
extern int L16;
extern int ledArray[];

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to setup GPIO as outupts.
 */
void setupOutputs(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to flash the provided pin.
 */
void flash(int pin);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to create a pulse train on the 16 test LEDs.
 */
void train(float aDelay);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to set the status LED to OFF, RED, GREEN, or BLUE.
 */
void setLED(int color);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to set the color of the status LED by passing in OFF, RED, GREEN, or BLUE.
 */
void setProtoboardLEDs(float aArray[]);

// End of file GPIO.h
