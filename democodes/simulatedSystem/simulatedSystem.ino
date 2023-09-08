#include <avdweb_VirtualDelay.h>
#define PIN_WATERLEVEL 10
#define PIN_MOTOR 11
#define MOTOR_OUT 9
#define MOTOR_IN 7

bool waterLevelStatus = false;
bool motorStatus = false;

VirtualDelay waterLevelDelay;
VirtualDelay motorDelay;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_WATERLEVEL, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(MOTOR_OUT, OUTPUT);
  pinMode(MOTOR_IN, INPUT_PULLUP);

  digitalWrite(PIN_WATERLEVEL, waterLevelStatus);
  digitalWrite(PIN_MOTOR, motorStatus);

  digitalWrite(MOTOR_OUT, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool motorRunning = digitalRead(MOTOR_IN);

  if (motorDelay.elapsed()) {
      motorStatus = motorRunning;
  }
  if (waterLevelDelay.elapsed()) {  //every few seconds flip the status of the simulated waterpump
    waterLevelStatus = !waterLevelStatus;
  }

  digitalWrite(PIN_WATERLEVEL, waterLevelStatus);
  digitalWrite(PIN_MOTOR, motorStatus);
  digitalWrite(LED_BUILTIN, waterLevelStatus);
  waterLevelDelay.start(30000);
  motorDelay.start(1500);
}
