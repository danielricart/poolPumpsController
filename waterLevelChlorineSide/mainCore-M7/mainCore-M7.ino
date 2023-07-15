#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif

#include <avdweb_VirtualDelay.h>
VirtualDelay updateSerialOutput;


void setup() {
  // put your setup code here, to run once:
  bootM4();
  updateSerialOutput.start(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);

  if (updateSerialOutput.elapsed()) {
    Serial.println("Hello from Main core...");
    updateSerialOutput.start(1000);
  }
}
