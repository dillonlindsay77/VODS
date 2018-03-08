/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Filename: Main.c
 * First Created: 2017-12-12
 * Last Modified: 2018-02-20
 * Author: LeddarTech / Kludge It Up
 *
 * Description:
 * This is a simple console program that will demonstrates the use of the
 * LeddarTech Evaluation Kit with the Capstone Design Project designed by
 * team Kludge It Up. The original LeddarSDK has been slightly modified to
 * provide the computations and additional functionality that the project
 * requirements demand.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Header files for normal LeddarSDK functionality.
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "LeddarC.h"
#include "LeddarProperties.h"

// Header files for additional VODS functionality.
#include <math.h>
#include <wiringPi.h>

#include "SensorInterpreter.h"
#include "GPIO.h"
#include "Sound.h"

// Get the length of an array.
#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

// Global variable to avoid passing to each function.
static LeddarHandle gHandle = NULL;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: CheckError
 *
 * Brief:
 * Check a return code and if it is not success, display an error message
 * corresponding to the code.
 *
 * Parameters:
 * aCode - The result code to verify.
 */
static void CheckError(int aCode)
{
  if (aCode != LD_SUCCESS) {
    LtChar lMessage[200];

    LeddarGetErrorMessage(aCode, lMessage, ARRAY_LEN(lMessage));
    LeddarPrintf(LTS("LeddarC error (%d): %s\n"), aCode, lMessage);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: WaitKey
 *
 * Description:
 * Wait for a key to be pressed on the keyboard, pinging the sensor to keep
 * the connection alive while waiting.
 *
 * Return:
 * The character corresponding to the key pressed (converted to uppercase
 * for letters).
 */
static char WaitKey(void)
{
  // LeddarGetKey is blocking so we need to wait for a key to be pressed
  // before calling it.
  while (!LeddarKeyPressed()) {
    // If a live connection is active we need to ping it periodically.
    if (LeddarGetConnected(gHandle)) {
      if (LeddarPing(gHandle) != LD_SUCCESS) {
        return 0;
      }
    }
    LeddarSleep(0.5);
  }
  return toupper(LeddarGetKey());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: DataCallback
 *
 * Description:
 * This is the function that is called when a new set of data is available.
 * Here we simply display the first 12 detections.
 *
 * Parameters:
 * aHandle - This is the user data parameter that was passed to
 *           LeddarAddCallback. Here by design we know its the handle.
 * aLevels - A bit mask of the data levels received in that frame.
 *
 * Return
 * Non zero to be called again (responding 0 would remove this function
 * from the callback list).
 */
static unsigned char DataCallback(void *aHandle, unsigned int aLevels)
{
  LdDetection lDetections[50];
  float detections[16];

  LeddarGetDetections(aHandle, lDetections, ARRAY_LEN(lDetections));

  // Flip the detection values to a new float array.
  for (int i = 0; i < ARRAY_LEN(detections); i++) {
    detections[i] = lDetections[ARRAY_LEN(detections)-1-i].mDistance;
  }

  // Print the angles.
  printArray(angs, ARRAY_LEN(angs));

  // Print the expectedR.
  printArray(expectedR, ARRAY_LEN(expectedR));

  // Print the distances measured.
  printArray(detections, ARRAY_LEN(detections));

  // Compute rErr and ho.
  calculateObjectHeight(detections);

  // Print ho.
  printArray(ho, ARRAY_LEN(ho));

  // Create space between sample printouts.
  puts("\n");

  // Operate protoboard LEDs.
  setProtoboardLEDs(ho);

  return 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: ReadLiveData
 *
 * Description:
 * Start data transfer until a key is pressed and stop it (data is
 * displayed by the callback).
 */
static void ReadLiveData(void)
{
  // puts("\nPress a key to start reading data and press a key again to stop.");
  // WaitKey();

  CheckError(LeddarStartDataTransfer(gHandle, LDDL_DETECTIONS));
  CheckError(LeddarAddCallback(gHandle, DataCallback, gHandle));

  WaitKey();

  LeddarStopDataTransfer(gHandle);
  LeddarRemoveCallback(gHandle, DataCallback, gHandle);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: ReadConfiguration
 *
 * Description:
 * Display some parameters of the current configuration.
 */
static void ReadConfiguration(void)
{
  double lValue;
  char lValueStr[64];

  puts("\n\nCurrent Configuration:\n");
  CheckError(LeddarGetTextProperty(gHandle, PID_NAME, 0, lValueStr, sizeof(lValueStr)));
  printf("  Device Name     : %s\n", lValueStr);
  CheckError(LeddarGetProperty(gHandle, PID_OVERSAMPLING, 0, &lValue));
  printf("  Oversampling    : %.0f\n", lValue);
  CheckError(LeddarGetProperty(gHandle, PID_ACCUMULATION, 0, &lValue));
  printf("  Accumulations   : %.0f\n", lValue);
  CheckError(LeddarGetProperty(gHandle, PID_BASE_POINT_COUNT, 0, &lValue));
  printf("  Base Point Count: %.0f\n", lValue);
  CheckError(LeddarGetProperty(gHandle, PID_LED_INTENSITY, 0, &lValue));
  printf("  Led Intensity   : %.0f\n", lValue);
  CheckError(LeddarGetProperty(gHandle, PID_THRESHOLD_OFFSET, 0, &lValue));
  printf("  Threshold offset: %.2f\n", lValue);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: ConfigurationMenu
 *
 * Description:
 * Menu allowing the change of configuration parameters.
 */
static void ConfigurationMenu(void)
{
  while (LeddarGetConnected(gHandle)) {
    char lChoice;
    unsigned int lId = 0;
    unsigned int lType = 1;

    puts("\n\nConfiguration Change Menu");
    puts("  1. Change Oversampling Exponent");
    puts("  2. Change Accumulation Exponent");
    puts("  3. Change Base Point Count");
    puts("  4. Change Led Intensity");
    puts("  5. Change Threshold Offset");
    puts("  6. Change Name");
    puts("  7. Write");
    puts("  8. Restore");
    puts("  9. Quit");

    lChoice = WaitKey();

    switch (lChoice) {
      case '1':
        lId = PID_OVERSAMPLING_EXPONENT;
        break;
      case '2':
        lId = PID_ACCUMULATION_EXPONENT;
        break;
      case '3':
        lId = PID_BASE_POINT_COUNT;
        break;
      case '4':
        lId = PID_LED_INTENSITY;
        break;
      case '5':
        lId = PID_THRESHOLD_OFFSET;
        break;
      case '6':
        lId = PID_NAME;
        lType = 2;
        break;
      case '7':
        CheckError(LeddarWriteConfiguration(gHandle));
        break;
      case '8':
        CheckError(LeddarRestoreConfiguration(gHandle));
        break;
      case '9':
      case  27: // Escape
        if (!LeddarGetConfigurationModified(gHandle)) {
          return;
        }

        puts("\n** Configuration modified, please Write or Restore before quitting **");
        break;
    }

    if (lId != 0) {
      printf("\nEnter new value: ");

      switch (lType) {
        case 1:
        {
          double lValue;
          scanf("%lf", &lValue);
          CheckError(LeddarSetProperty(gHandle, lId, 0, lValue));
        }
          break;
        case 2:
        {
          char lValue[64];
          scanf("%63s", lValue);
          CheckError(LeddarSetTextProperty(gHandle, lId, 0, lValue));
        }
          break;
      }
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: ConnectMenu
 *
 * Description:
 * Main menu when a live connection is made.
 *
 * Parameters:
 * aTrySingleUsb - If true we will try to connect to a single USB sensor by
 *                 sending an empty string as the address. This works only
 *                 if there is 1 and only 1 USB sensor plugged to the PC.
 */
static void ConnectMenu(int aTrySingleUsb)
{
  char lAddress[24];

  if (aTrySingleUsb) {
    lAddress[0] = 0;
  } else {
    // Ask for address and try to connect before displaying menu.
    printf("\nEnter address: ");
    scanf("%24s", lAddress);
  }

  if (LeddarConnect(gHandle, lAddress) == LD_SUCCESS) {
    while (LeddarGetConnected(gHandle)) {
      char lChoice;
      float ans;

      puts("\nConnected Menu");
      puts("  1. Read Configuration");
      puts("  2. Change Configuration");
      puts("  3. Set Sensor Height");
      puts("  4. Set Center Beam Angle");
      puts("  5. Back");

      lChoice = WaitKey();

      switch (lChoice) {
        case '1':
          ReadConfiguration();
          break;
        case '2':
          ConfigurationMenu();
          break;
        case '3':
          puts("Enter Height: ");
          scanf("%f", &ans);
          setSensorHeight(ans);
          break;
        case '4':
          puts("Enter Center Beam Angle (deg): ");
          scanf("%f", &ans);
          setCenterAngle(ans);
          break;
        case '5':
        case  27:
          LeddarDisconnect(gHandle);
          return;
      }
    }
  } else {
    puts("\nConnection failed!");
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: ListSensors
 *
 * Description:
 * List the address of all sensors available.
 */
static void ListSensors(void)
{
  char lAddresses[256];
  unsigned int lCount = sizeof(lAddresses);
  unsigned int lIndex = 0;

  puts("\nScanning for available sensors, please wait...");

  CheckError(LeddarListSensors(lAddresses, &lCount, 2000));

  printf("Found %d sensors\n", lCount);

  while (strlen(lAddresses+lIndex) > 0) {
    printf("%s\n", lAddresses + lIndex);
    lIndex += strlen(lAddresses+lIndex) + 1;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: TestSoundFiles
 *
 * This function will give some the option to test the sound fies.
 */
static void TestSoundFiles(void)
{
  for (;;) {

    char lChoice;

    puts("\nTest Sound Files");
    puts("  1. Play Dead Battery");
    puts("  2. Play Drop Off");
    puts("  3. Play Overhead");
    puts("  4. Play Ramp Down");
    puts("  5. Play Ramp Up");
    puts("  6. Play Stairs Down");
    puts("  7. Play Stairs Up");
    puts("  8. Play Step Down");
    puts("  9. Play Step Up");
    puts("  0. Play Wall");
    puts("  Q. Quit");

    lChoice = toupper(LeddarGetKey());

    switch (lChoice) {
      case '1':
        playDeadBattery();
        break;
      case '2':
        playDropOff();
        break;
      case '3':
        playOverhead();
        break;
      case '4':
        playRampDown();
        break;
      case '5':
        playRampUp();
        break;
      case '6':
        playStairsDown();
        break;
      case '7':
        playStairsUp();
        break;
      case '8':
        playStepDown();
        break;
      case '9':
        playStepUp();
        break;
      case '0':
        playWall();
        break;
      case 'Q':
        return;
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: VODS
 *
 * This function will handle how the project runs.
 */
static void VODS(void)
{
  puts("\n\n\n\nEntering VODS Function.");

  // Calculation Setup
  setCenterAngle(22.5);
  setSensorHeight(1.27);
  calculateExpectedR();

  setLED(RED);

  // Test protoboard LEDs.
  train(62.5);

  // Try to connect to LeddarTech Sensor.
  setLED(YELLOW);

  char lAddress[24];
  lAddress[0] = 0;
  sleep(1);

  while (LeddarConnect(gHandle, lAddress) != LD_SUCCESS) {
    puts("\nScanning for available sensors, please wait...");
    sleep(1);
  }

  puts("Connected!");
  setLED(GREEN);
  sleep(1);

  // Display Data.
  ReadLiveData();

  // Exit function
  setLED(RED);
  puts("Exiting VODS function.\n");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: MainMenu
 *
 * Description:
 * Display and responds to the main menu.
 */
static void MainMenu(void)
{
  for (;;) {

    int lChoice;

    setLED(RED);

    puts("\nMain Menu");
    puts("  1. VODS");
    puts("  2. Test Sound Files");
    puts("  3. Turn Volume Down");
    puts("  4. Turn Volume Up");
    puts("  5. Change Sensor Parameters");
    puts("  6. List Sensors");
    puts("  Q. Quit");

    lChoice = toupper(LeddarGetKey());

    switch (lChoice) {
      case '1':
        VODS();
        break;
      case '2':
        TestSoundFiles();
        break;
      case '3':
        turnVolumeDown();
        break;
      case '4':
        turnVolumeUp();
        break;
      case '5':
        ConnectMenu(1);
        break;
      case '6':
        ListSensors();
        break;
      case 'Q':
      case  27:
        setLED(OFF);
        puts("\n***Goodbye***");
        return;
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function: main
 *
 * Description:
 * Standard C entry point!
 */
int main(int argc, char *argv[])
{
  // Setup GPIO
  wiringPiSetup();
  setupOutputs();

  puts("\n");
  puts("===============================================");
  puts("|  Vertical Obstacle Detection System - VODS  |");
  puts("===============================================");

  gHandle = LeddarCreate();

  MainMenu();

  LeddarDestroy(gHandle);

  return 0;
}

// End of file Main.c

