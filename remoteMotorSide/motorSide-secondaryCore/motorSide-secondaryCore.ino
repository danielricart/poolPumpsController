//MotorSide SecondayCore
#ifdef CORE_CM7
#error "This Code is for CORE_CM4 only"
#endif

#include <avdweb_VirtualDelay.h>
#include <EasyButton.h>
#include "RPC.h"

#define PIN_MOTOR_STATUS A0
#define PIN_MOTOR RELAY1
#define LED_REMOTE_WATER_LEVEL LED_D0
#define LED_MOTOR_STATUS LED_D1
#define LED_REMOTE_CHLORINE_STATUS LED_D2
#define LED_OVERRIDE LED_D3
#define BUTTON_LONG_PRESS 2000

int remoteWaterLevel = LOW;
int remoteChlorineStatus = LOW;
int MotorStatus = LOW;
int turnMotorOn = LOW;

EasyButton button(BTN_USER);
bool overrideBehaviour = false;

VirtualDelay updateSerialOutput;

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
}

void setup() {
  // put your setup code here, to run once:
  RPC.begin();
  delay(200);


  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(LED_REMOTE_WATER_LEVEL, OUTPUT);
  pinMode(LED_MOTOR_STATUS, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
  pinMode(BTN_USER, INPUT_PULLUP);
  pinMode(PIN_MOTOR_STATUS, INPUT);

  button.begin();
  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.read();

  auto remoteWaterLevel = RPC.call("getRemoteWaterLevel").as<int>();
  auto remoteChlorineStatus = RPC.call("getRemoteChlorineStatus").as<int>();
  auto turnMotorOn = RPC.call("getTurnMotorOn").as<int>();

  turnMotorOn = turnMotorOn || overrideBehaviour;
  /*  int sensorValueA0 = analogRead(PIN_MOTOR_STATUS);
  float voltageA0 = sensorValueA0 * (3.0 / 4095.0) / 0.3;
  MotorStatus = (voltageA0 < 2.0f) || overrideBehaviour;
*/
  MotorStatus = digitalRead(PIN_MOTOR_STATUS);



  digitalWrite(LED_REMOTE_WATER_LEVEL, remoteWaterLevel);
  RPC.send("setMotorStatus", MotorStatus);
  digitalWrite(LED_MOTOR_STATUS, MotorStatus);
  digitalWrite(LED_REMOTE_CHLORINE_STATUS, remoteChlorineStatus);
  digitalWrite(LED_OVERRIDE, overrideBehaviour);
  digitalWrite(PIN_MOTOR, turnMotorOn);
}
