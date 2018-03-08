/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: SensorInterpreter.h
 * First Created: 2017-12-26
 * Last Modified: 2018-01-29
 * Author: Dillon Lindsay
 *
 * Description:
 * This file contains all of the declerations used in to interpret the
 * sensor values as object heights.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables for distance calculations.
 */
float angs[16];
float expectedR[16];
float rErr[16];
float ho[16];
float anglePerBeam; // Angle of each beam ie. 45deg/16beams = 2.8125deg/beam

float centerAngle; // Angle from floor to center beam in degrees.
float hs; // height of the sensor.

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Setters for all changing variables.
 */
void setCenterAngle(float aCenterValue);

void setSensorHeight(float aSensorHeight);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to calculate the values for angs[] and expectedR[].
 */
void calculateExpectedR(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to calculate the values for ho[].
 */
void calculateObjectHeight(float aDetections[]);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to print the values in an array.
 */
void printArray(float aArray[], int aLength);

// End of file SensorInterpreter.h

