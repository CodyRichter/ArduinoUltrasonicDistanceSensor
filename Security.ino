//
// Basic Software Setup
//
double softwareVersion = 1.0;

//
// Display Setup and Controls
//
int displayDataPins[] = {8,9,10,11}; //Display Data Pins: (4 Total)
int registerPin = 2, enablePin = 3; //Display Control Pins
#include <LiquidCrystal.h> //Includes LCD Display Library
LiquidCrystal lcd(registerPin, enablePin, displayDataPins[0], displayDataPins[1], displayDataPins[2], displayDataPins[3]);

//
// Data Logging
//
int currentTime[] = {0,0,0}; //Current Time: Set in Hours-Minutes-Seconds Out of 24 Hours (Military Time)
int currentDate[] = {0,0}; //Current Date: Set in Month-Day
int infractions[100][4]; //Stores Up to 100 Infractions, Stored In Format: [Month][Day][Time (s)][Duration (s)]

//
//Software Control Variables
//

void setup() {
lcd.begin(16, 2); //Sets up 16x2 LCD Display
lcd.print("Starting Setup..");
lcd.setCursor(0, 1);
lcd.print("Version: "); lcd.print(softwareVersion);
}

void loop() {
//Motion Detection

//Mode 0: Passcode Input

//Mode 1: Log Access

//Mode 2: Password Controls

//Mode 3: Setup


}
