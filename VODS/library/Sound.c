/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: Sound.c
 * First Created: 2018-02-11
 * Last Modified: 2018-02-19
 * Author: Dillon Lindsay
 *
 * Description:
 * This file contains all of the definitions used for any sounds.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Dead Battery WAV file.
 */
void playDeadBattery(void)
{
  system("aplay /home/pi/VODS/sounds/DeadBattery.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Drop Off WAV file.
 */
void playDropOff(void)
{
  system("aplay /home/pi/VODS/sounds/DropOff.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Ms. Pacman Intro WAV file.
 */
void playMsPacmanIntro(void)
{
  system("aplay /home/pi/VODS/sounds/MsPacmanIntro.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Overhead WAV file.
 */
void playOverhead(void)
{
  system("aplay /home/pi/VODS/sounds/Overhead.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Ramp Down WAV file.
 */
void playRampDown(void)
{
  system("aplay /home/pi/VODS/sounds/RampDown.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Ramp Up WAV file.
 */
void playRampUp(void)
{
  system("aplay /home/pi/VODS/sounds/RampUp.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Stairs Down WAV file.
 */
void playStairsDown(void)
{
  system("aplay /home/pi/VODS/sounds/StairsDown.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Stairs Up WAV file.
 */
void playStairsUp(void)
{
  system("aplay /home/pi/VODS/sounds/StairsUp.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Step Down WAV file.
 */
void playStepDown(void)
{
  system("aplay /home/pi/VODS/sounds/StepDown.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Step Up WAV file.
 */
void playStepUp(void)
{
  system("aplay /home/pi/VODS/sounds/StepUp.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to play the Wall WAV file.
 */
void playWall(void)
{
  system("aplay /home/pi/VODS/sounds/Wall.wav&");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to turn playback volume down.
 */
void turnVolumeDown(void)
{
  // PCM ranges from -10239 - +400. Divide into 16 sound levels.
  system("amixer set PCM 664.9375-");
  playDeadBattery();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function to turn playback volume up.
 */
void turnVolumeUp(void)
{
  // PCM ranges from -10239 - +400. Divide into 16 sound levels.
  system("amixer set PCM 664.9375+");
  playDeadBattery();
}

// End of file Sound.c
