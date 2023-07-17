#ifdef CORE_CM7
#error "This Code is for CORE_CM4 only"
#endif

#include <EasyButton.h>
#include "RPC.h"


#define BUTTON_LONG_PRESS 2000
#define LED_OVERRIDE LED_D3

#define PIN_WATERLEVEL A0
#define LED_WATERLEVEL LED_D0

#define LED_REMOTEMOTOR LED_D1

#define PIN_CHLORINE RELAY1
#define LED_CHLORINE LED_D2

EasyButton button(BTN_USER);
bool overrideBehaviour = false;

bool waterLevelStatus = false;
bool newWaterLevelStatus = false;
unsigned long lastWaterLevelStatusTransition;

bool turnRemoteMotorOn = false;

bool remoteMotorStatus = false;
bool newRemoteMotorStatus = false;
unsigned long lastRemoteMotorStatusTransition;

bool chlorineStatus = false;

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OVERRIDE, OUTPUT);
  digitalWrite(LED_OVERRIDE, LOW);

  pinMode(LED_WATERLEVEL, OUTPUT);
  pinMode(PIN_WATERLEVEL, INPUT);

  pinMode(LED_CHLORINE, OUTPUT);
  digitalWrite(LED_CHLORINE, LOW);

  pinMode(PIN_CHLORINE, OUTPUT);
  digitalWrite(PIN_CHLORINE, LOW);

  RPC.begin();
  // Initialize the button.
  delay(200);
  button.begin();
  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);
}

void loop() {
  // READ INPUTS
  unsigned long now = millis();
  button.read();
  /*int sensorValueA0 = analogRead(PIN_WATERLEVEL);
  float voltageA0 = sensorValueA0 * (3.0 / 4095.0) / 0.3;
  newWaterLevelStatus = (voltageA0 < 2.0f) || overrideBehaviour;
*/
  newWaterLevelStatus = !digitalRead(PIN_WATERLEVEL) || overrideBehaviour;  
  auto remoteMotorResult = RPC.call("getRemoteMotorStatus").as<int>();
  newRemoteMotorStatus = (bool)remoteMotorResult;
  // EVALUATE

  //TODO: ADD Delay so it does not immediately react to changes in waterLevelStatus
  if (newWaterLevelStatus != waterLevelStatus) {
    lastWaterLevelStatusTransition = now;
  }
  if ((now - lastWaterLevelStatusTransition) > 1000) {  // THis delay debounces the reading and delays reactions.
    turnRemoteMotorOn = waterLevelStatus;
  }
  waterLevelStatus = newWaterLevelStatus;


  //TODO: ADD Delay so it does not immediately react to changes in remoteMotorStatus
  if (newRemoteMotorStatus != remoteMotorStatus) {
    lastRemoteMotorStatusTransition = now;
  }
  if (now - lastRemoteMotorStatusTransition > 1000) {  // THis delay debounces the reading and delays reactions.
    chlorineStatus = remoteMotorStatus;
  }
  remoteMotorStatus = newRemoteMotorStatus;


  // WRITE OUTPUTS
  digitalWrite(LED_WATERLEVEL, waterLevelStatus);
  digitalWrite(LED_REMOTEMOTOR, remoteMotorStatus);
  digitalWrite(LED_CHLORINE, chlorineStatus);
  digitalWrite(PIN_CHLORINE, chlorineStatus);
  digitalWrite(LED_OVERRIDE, overrideBehaviour);

  RPC.send("setVariables", waterLevelStatus, chlorineStatus, turnRemoteMotorOn, overrideBehaviour);
}
