#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif
void setup() {
  // put your setup code here, to run once:
  bootM4();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
}
