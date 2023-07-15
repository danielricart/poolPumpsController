#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif

#include <avdweb_VirtualDelay.h>
#include "RPC.h"


VirtualDelay updateSerialOutput;

bool waterLevel = false;
bool chlorineStatus = false;
bool motorOn = false;
bool remoteMotorStatus = false;
bool overrideBehaviour = false; 

void setVariables(bool newWaterLevel, bool newChlorineStatus, bool newMotorOn, bool newOverride) {
  waterLevel = newWaterLevel;
  chlorineStatus = newChlorineStatus;
  motorOn = newMotorOn;
  overrideBehaviour = newOverride;
}

int getRemoteMotorStatus() {
  return remoteMotorStatus;
}

void setup() {
  // put your setup code here, to run once:
  bootM4();
  updateSerialOutput.start(1000);
  RPC.begin();
  RPC.bind("setVariables", setVariables);
  RPC.bind("getRemoteMotorStatus", getRemoteMotorStatus);
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);

  if (updateSerialOutput.elapsed()) {
    Serial.print("waterLevel: ");
    Serial.print(waterLevel);
    Serial.print("  chlorineStatus: ");
    Serial.print(chlorineStatus);
    Serial.print("  motorOn: ");
    Serial.print(motorOn);
    Serial.print("  remoteMotorStatus: ");
    Serial.print(remoteMotorStatus);
    Serial.print("  overrideBehaviour: ");
    Serial.print(overrideBehaviour);

    Serial.println(" ");
    updateSerialOutput.start(1000);
  }
}
