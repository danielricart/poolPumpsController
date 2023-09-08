void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  bool A0_status = digitalRead(A0);
  bool A1_status = digitalRead(A1);
  
  digitalWrite(LED_D0, A0_status);
  digitalWrite(LED_D1, A1_status);
  
}
