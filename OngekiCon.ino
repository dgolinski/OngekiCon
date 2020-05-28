/* Code based off example code provided by David Madison's GamepadPins example included in the Xinput Library

   Before you can use this code, make sure to download needed libraries
   - https://github.com/dmadison/ArduinoXInput
   - https://github.com/FastLED/FastLED

   This file was created with Arduino Leonardo board in mind

   USAGE:
   this will cover your basic XInput controller for ONGEKI

   ONGEKI's XInput lever is processed as when neither trigger is pressed, your character is in the center of the screen.
   Press the left side and it moves left, release and it returns to center. Same with the right side

   I set the lever range to 519 which is an approximate halfway point between the min and max values of my potentiometer.
   That half way point has to be calculated because you're splitting one input method between two "triggers"
   I also set the lever calculations to work around that half way point.


   Note: Removing XInput in place of keyboard and mouse
   Also removed leftmenu and rightmenu pins because my con does not include those buttons

   May re-add FastLED support later
*/

//#include <XInput.h>
//#include <FastLED.h>

#include <Keyboard.h>
#include <Mouse.h>


// Lever Setup

int leverRange = 519; //519
int positionA = 0;
int positionB = 0;
int previousPosition = 0;
int currentPosition = 0;

// Lever Pins
const int Pin_Lever = A0;

// Button Setup
bool buttonsPressed[] = {false, false, false, false, false, false, false, false};
bool recentState[] = {false, false, false, false, false, false, false, false};
const char keyBinds[] = {'s', 'd', 'f', 'j', 'k', 'l', 'a', ';'};

// Button Pins
const int Pin_LeftA = 2;
const int Pin_LeftB = 3;
const int Pin_LeftC = 4;
const int Pin_RightA = 5;
const int Pin_RightB = 6;
const int Pin_RightC = 7;
const int Pin_LeftSide = 8;
const int Pin_RightSide = 9;

void setup()
{
  // Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_LeftA, INPUT_PULLUP);
  pinMode(Pin_LeftB, INPUT_PULLUP);
  pinMode(Pin_LeftC, INPUT_PULLUP);
  pinMode(Pin_RightA, INPUT_PULLUP);
  pinMode(Pin_RightB, INPUT_PULLUP);
  pinMode(Pin_RightC, INPUT_PULLUP);
  pinMode(Pin_LeftSide, INPUT_PULLUP);
  pinMode(Pin_RightSide, INPUT_PULLUP);
  Keyboard.begin();
  Mouse.begin();
}

void loop()
{

  // Read pin values and store in variables
  int lever = analogRead(Pin_Lever);
  bool leftA = !digitalRead(Pin_LeftA);
  bool leftB = !digitalRead(Pin_LeftB);
  bool leftC = !digitalRead(Pin_LeftC);
  bool rightA = !digitalRead(Pin_RightA);
  bool rightB = !digitalRead(Pin_RightB);
  bool rightC = !digitalRead(Pin_RightC);
  bool leftSide = !digitalRead(Pin_LeftSide);
  bool rightSide = !digitalRead(Pin_RightSide);

  //  Lever Calibration
  bool leverCalibrated = false;
  int leverMaxLeft;
  int leverMaxRight;
  
  while (leverCalibrated == false)
  { //Reminder: Test to see if the value returned is an integer or a float
    leftA = !digitalRead(Pin_LeftA);
    if (leftA == true)
    {
      lever = analogRead(Pin_Lever);
      leverMaxLeft = lever;
      while (leverCalibrated == false)
      {
        rightA = !digitalRead(Pin_RightA);
        if (rightA == true)
        {
          lever = analogRead(Pin_Lever);
          leverMaxRight = lever;
          leverCalibrated = true;
        }
      }
    }
  }

  leverRange = (leverMaxLeft + leverMaxRight) / 2;

  //Main loop
  while (1)
  {
    lever = analogRead(Pin_Lever);
    leftA = !digitalRead(Pin_LeftA);
    leftB = !digitalRead(Pin_LeftB);
    leftC = !digitalRead(Pin_LeftC);
    rightA = !digitalRead(Pin_RightA);
    rightB = !digitalRead(Pin_RightB);
    rightC = !digitalRead(Pin_RightC);
    leftSide = !digitalRead(Pin_LeftSide);
    rightSide = !digitalRead(Pin_RightSide);

    // Calculate lever values
    if (lever < leverRange)
    {
      positionB = -(lever - leverRange);
    }
    else if (lever > leverRange)
    {
      positionA = lever - leverRange;
    }
    else
    {
      positionA = 0;
      positionB = 0;
    }

    currentPosition = positionA + positionB;

    if (previousPosition != currentPosition)
    {
      Mouse.move((currentPosition - previousPosition),0,0);
      previousPosition = currentPosition;
    }

    bool buttonsPressed[] = {leftA, leftB, leftC, rightA, rightB, rightC, leftSide, rightSide};

    for (int i = 0; i < 8; i++)
    {
      if (recentState[i] != buttonsPressed[i])
      {
        if (buttonsPressed[i] == true)
        {
          if (buttonsPressed[i] == true)
          {
            Keyboard.press(keyBinds[i]);
          }
          else
          {
            Keyboard.release(keyBinds[i]);
          }
        }
    }
    memcpy( recentState, buttonsPressed, 5 );
  }
}
}
