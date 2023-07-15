#ifdef CORE_CM4
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D3, HIGH);
}
#else
bool led0 = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println(" Main core booted. Booting Secondary core...");
  bootM4();
  Serial.println("Secondary core booted.");
}

void loop() {
  digitalWrite(LED_D0, led0);
  led0 = !led0;
  delay(100);
}
#endif