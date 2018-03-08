/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: SensorInterpreter.c
 * First Created: 2017-12-26
 * Last Modified: 2018-01-29
 * Author: Dillon Lindsay
 *
 * Description:
 * This file contains all of the definitions used in to interpret the
 * sensor values as object heights.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <math.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Variables for distance calculations.
 */
float angs[16];
float expectedR[16];
float rErr[16];
float ho[16];
float angle = 2.8125; // Angle of each beam ie. 45deg/16beams = 2.8125deg/beam

float centerAngle = 0; // Angle from floor to center beam in degrees.
float hs = 0; // height of the sensor.

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Setters for all changing variables.
 */
void setCenterAngle(float aCenterAngle)
{
  centerAngle = aCenterAngle;
}

void setSensorHeight(float aSensorHeight)
{
  hs = aSensorHeight;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to calculate the values for angs[] and expectedR[].
 */
void calculateExpectedR(void)
{
  // Populate the angles array from the center angle.
  for(int i = -8; i < 8; i++) {
      angs[i+8] = i*angle + centerAngle;
  }
  // Calculate the expected distance r.
  for(int i = 0; i < 16; i++) {
      expectedR[i] = hs/cos(angs[i]*3.14159/180);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to calculate the values for ho[].
 */
void calculateObjectHeight(float aDetections[])
{
  for(int i = 0; i < 16; i++) {
    rErr[i] = expectedR[i]-aDetections[i];
    ho[i] = rErr[i]*cos(angs[i]*3.14159/180);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to print the values in an array.
 */
void printArray(float aArray[], int aLength)
{
    for(int i = 0; i < aLength; i++) {
      printf("%5.2f ", aArray[i]);
    }
    puts("");
}

// End of file SensorInterpreter.c
