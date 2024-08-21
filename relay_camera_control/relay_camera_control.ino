#include <Nextion.h>
#include <NexButton.h>
#include <NexNumber.h>
// Pin definitions
const int relayPin = A5;
const int button2Pin = 10;

// Variables to hold time values
unsigned long shutterTime = 1000;  // Default to 1 second
unsigned long startTime;
bool relayActive = false;

// Nextion display objects
NexButton bPlus = NexButton(0, 2, "bPlus");
NexButton bMinus = NexButton(0, 1, "bMinus");
NexNumber tTime = NexNumber(0, 3, "tTime");
NexButton bShutter = NexButton(0, 4, "bShutter");

// List of shutter speeds in milliseconds
unsigned long shutterSpeeds[] = { 1667, 3333, 6667, 125, 250, 500, 1000, 2000, 3000, 4000, 118000, 119000, 120000 };
int currentShutterSpeedIndex = 6;  // Default to 1 second (index 6)

// Function to update the display with the current shutter time
void updateDisplay() {
  char buffer[10];
  if (shutterTime < 1000) {
    sprintf(buffer, "%04lu ms", shutterTime);
  } else {
    sprintf(buffer, "%04lu s", shutterTime / 1000);
  }
  tTime.setValue(buffer);
}

// Function to handle the "+" button press
void bPlusPushCallback(void *ptr) {
  if (currentShutterSpeedIndex < (sizeof(shutterSpeeds) / sizeof(shutterSpeeds[0])) - 1) {
    currentShutterSpeedIndex++;
    shutterTime = shutterSpeeds[currentShutterSpeedIndex];
    updateDisplay();
  }
}

// Function to handle the "-" button press
void bMinusPushCallback(void *ptr) {
  if (currentShutterSpeedIndex > 0) {
    currentShutterSpeedIndex--;
    shutterTime = shutterSpeeds[currentShutterSpeedIndex];
    updateDisplay();
  }
}

// Function to handle the shutter button press
void bShutterPushCallback(void *ptr) {
  digitalWrite(relayPin, HIGH);
  delay(shutterTime);
  digitalWrite(relayPin, LOW);
}

// Button object list for easier management
NexTouch *nex_listen_list[] = {
  &bPlus,
  &bMinus,
  &bShutter,
  &tTime,
  NULL
};

void setup() {
  // Set pin modes
  pinMode(relayPin, OUTPUT);
  pinMode(button2Pin, INPUT_PULLUP);

  // Initialize relay as off
  digitalWrite(relayPin, LOW);

  // Initialize Nextion display
  nexInit();

  // Register button callbacks
  bPlus.attachPush(bPlusPushCallback);
  bMinus.attachPush(bMinusPushCallback);
  bShutter.attachPush(bShutterPushCallback);

  // Initialize display with default time
  updateDisplay();
}

void loop() {
  nexLoop(nex_listen_list);

  // Manual shutter control with Button 2
  if (digitalRead(button2Pin) == LOW) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}
