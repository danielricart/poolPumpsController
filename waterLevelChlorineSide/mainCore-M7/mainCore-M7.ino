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

void setVariables(bool newWaterLevel, bool newChlorineStatus, bool newMotorOn) {
  waterLevel = newWaterLevel;
  chlorineStatus = newChlorineStatus;
  motorOn = newMotorOn;
}

bool getRemoteMotorStatus() {
  return remoteMotorStatus;
}

void setup() {
  // put your setup code here, to run once:
  bootM4();
  updateSerialOutput.start(1000);
  RPC.begin();
  RPC.bind("setVariables", setVariables);
  RPC.bind("getRemoteMotorStatus", getRemoteMotorStatus);
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

    Serial.println(" ");
    updateSerialOutput.start(1000);
  }
}
