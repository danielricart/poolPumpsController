#ifdef CORE_CM4
#include "RPC.h"

bool led1_status = false;
void switchLedD1(bool newStatus) {
  led1_status = newStatus;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
  RPC.begin();
  RPC.bind("switchLedD1", switchLedD1);
}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D3, HIGH);
  digitalWrite(LED_D1, led1_status);
}
#else
#include "RPC.h"

bool led0 = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println(" Main core booted. Booting Secondary core...");
  delay(100);
  bootM4();
  RPC.begin();
  delay(100);
  Serial.println("Secondary core booted.");
}

void loop() {
  digitalWrite(LED_D0, led0);
  led0 = !led0;
  delay(100);
  RPC.send("switchLedD1", !led0);
}
#endif