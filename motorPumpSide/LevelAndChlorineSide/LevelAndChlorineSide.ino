/*
If WaterLevel then turn on remote motor
When remote motor is ON, turn on the local chlorine pump
*/

/*
Connections:
I1 -> Water level sensor. LOW-->TURN ON EVERYTHING. 
RELAY1 -> Chlorine pump

LEDS:
RESET: Green-> Connection established to remote board
RESET: Red -> Cannot connect to remote board

STATUS_1: WaterLevel Sensor is read ON
STATUS_2: Remote Motor is ON (INPUT=LOW -> MOTOR IS RUNNING)
STATUS_3: Chlorine is ON
STATUS_4: NC

Buttons:
USER: NC


*/
#ifdef CORE_CM7 
#error "This Code is for CORE_CM4 only"
#endif
#include <avdweb_VirtualDelay.h>
#include <EasyButton.h>
#include "RPC.h"  // comes with the mbed board installation


#define LEVEL_SIGNAL PIN_A1
#define LEVEL_SIGNAL_LED LED_D0
#define MOTOR_LED LED_D1
#define CHLORINE_LED LED_D2
#define CHLORINE_PIN RELAY2

#define OVERRIDE_LED LED_D3

#define MOTOR_DELAY_ON 5000   // milliseconds to wait between PUMP_SIGNAL == HIGH and MOTOR_PIN = HIGH
#define MOTOR_DELAY_OFF 3000  // milliseconds to wait between PUMP_SIGNAL==LOW and MOTOR_PIN = LOW

#define CHLORINE_DELAY_ON 5000
#define CHLORINE_DELAY_OFF 3000

#define RPC_UPDATE_DELAY 700
#define STABILIZATION_DELAY 1000  //Time between a transition/status change (local or remote) and that value is accepted/persisted

#define BUTTON_LONG_PRESS 2000


int waterLevel = LOW;
int chlorineStatus = LOW;
int turnMotorOn = LOW;
int remoteMotorStatus = LOW;

VirtualDelay delayMotorOn;
VirtualDelay delayMotorOff;
VirtualDelay delayChlorineOn;
VirtualDelay delayChlorineOff;
VirtualDelay updateSecondaryCore;

unsigned long lastWaterLevelTransition = 0;
unsigned long lastChlorineStatusTransition = 0;
unsigned long lastRemoteMotorStatusTransition = 0;

bool overrideBehaviour = false;

EasyButton button(BTN_USER);

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
  waterLevel = !waterLevel;
  if (overrideBehaviour) {
    Serial.print("Behaviour is overridden. going manual. ");
    Serial.print("waterLevel is ");
    Serial.println(waterLevel);
  } else
    Serial.println("Behaviour is automatic");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LEVEL_SIGNAL, INPUT_PULLUP);
  pinMode(LEVEL_SIGNAL_LED, OUTPUT);
  pinMode(MOTOR_LED, OUTPUT);
  pinMode(CHLORINE_LED, OUTPUT);
  pinMode(CHLORINE_PIN, OUTPUT);
  pinMode(OVERRIDE_LED, OUTPUT);

  digitalWrite(LEVEL_SIGNAL_LED, LOW);
  digitalWrite(MOTOR_LED, LOW);
  digitalWrite(CHLORINE_PIN, LOW);
  digitalWrite(OVERRIDE_LED, LOW);

  RPC.begin();
  delay(1000);
  updateSecondaryCore.start(RPC_UPDATE_DELAY);

  // Initialize the button.
  button.begin();

  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);

}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();
  //READ
  int newWaterLevelStatus = digitalRead(LEVEL_SIGNAL);
  auto newRemoteMotorStatus = RPC.call("getRemoteMotorStatus").as<int>();

  if (waterLevel != newWaterLevelStatus) {
    lastWaterLevelTransition = currentTime;
  } else if (currentTime - lastWaterLevelTransition >= STABILIZATION_DELAY && !overrideBehaviour) {
    waterLevel = newWaterLevelStatus;
  }
  if (remoteMotorStatus != newRemoteMotorStatus) {
    lastRemoteMotorStatusTransition = currentTime;
  } else if (currentTime - lastRemoteMotorStatusTransition >= STABILIZATION_DELAY) {
    remoteMotorStatus = newRemoteMotorStatus;
  }

  // ACT
  if (delayMotorOn.elapsed()) {
    turnMotorOn = waterLevel;
    Serial.print("Wait time to turn on motor past. Motor is ");
    Serial.println(remoteMotorStatus);
  }
  if (delayMotorOff.elapsed()) {
    turnMotorOn = waterLevel;
    Serial.print("Wait time to turn off motor past. Motor is ");
    Serial.println(remoteMotorStatus);
  }

  if (delayChlorineOn.elapsed()) {
    chlorineStatus = remoteMotorStatus;
    Serial.print("Wait time to turn on chlorine past. Chlorine is ");
    Serial.println(chlorineStatus);
  }
  if (delayChlorineOff.elapsed()) {
    chlorineStatus = remoteMotorStatus;
    Serial.print("Wait time to turn off chlorine past. Chlorine is ");
    Serial.println(chlorineStatus);
  }

  if (waterLevel) {
    delayMotorOn.start(MOTOR_DELAY_ON);
    delayMotorOff.running = false;
    //Serial.println("Pump is ON");
  } else {
    delayMotorOn.running = false;
    delayMotorOff.start(MOTOR_DELAY_OFF);
    //Serial.println("Pump is OFF");
  }

  if (remoteMotorStatus) {
    delayChlorineOn.start(CHLORINE_DELAY_ON);
    delayChlorineOff.running = false;
  } else {
    delayChlorineOn.running = false;
    delayChlorineOff.start(CHLORINE_DELAY_OFF);
  }


  //OUTPUT
  digitalWrite(LEVEL_SIGNAL_LED, waterLevel);
  digitalWrite(MOTOR_LED, remoteMotorStatus);
  digitalWrite(CHLORINE_LED, chlorineStatus);
    digitalWrite(OVERRIDE_LED, overrideBehaviour);

  if (updateSecondaryCore.elapsed()) {
    RPC.send("setWaterLevelStatus", waterLevel);
    RPC.send("setChlorineStatus", chlorineStatus);
    RPC.send("setTurnMotorOn", turnMotorOn);
    updateSecondaryCore.start(RPC_UPDATE_DELAY);
  }
}
