#ifdef CORE_CM4
//#error "This Code is for CORE_CM7 only"
#endif
#include "RPC.h"  // comes with the mbed board installation

int waterLevel = LOW;
int chlorineStatus = LOW;
int remoteMotorStatus = LOW;
int turnMotorOn = LOW;

bool etherStatus = false;
bool remoteComms = false;

void setWaterLevelStatus(int newStatus) {
  waterLevel = newStatus;
}

void setChlorineStatus(int newStatus) {
  chlorineStatus = newStatus;
}

void setTurnMotorOn(int newStatus) {
  turnMotorOn = newStatus;
}

int getRemoteMotorStatus() {
  return remoteMotorStatus;
}

unsigned long lastUpdate;
void setup() {
  // put your setup code here, to run once:
  RPC.begin();
  RPC.bind("setWaterLevelStatus", setWaterLevelStatus);
  RPC.bind("setChlorineStatus", setChlorineStatus);
  RPC.bind("setTurnMotorOn", setTurnMotorOn);
  RPC.bind("getRemoteMotorStatus", getRemoteMotorStatus);
  #ifdef CORE_CM7
  bootM4();
  Serial.println("Both cores are booted up");
  #endif
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastUpdate > 1000) {
    Serial.print("Status: ");
    Serial.print(" waterLevel: ");
    Serial.print(waterLevel);
    Serial.println(" ");
  }
  digitalWrite(LEDG, waterLevel);
}
