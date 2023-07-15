#ifdef CORE_CM7
#error "This Code is for CORE_CM4 only"
#endif

#include <EasyButton.h>

#define BUTTON_LONG_PRESS 2000
#define LED_OVERRIDE LED_D3

EasyButton button(BTN_USER);
bool overrideBehaviour = false;

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OVERRIDE, OUTPUT);
  digitalWrite(LED_OVERRIDE, LOW);

  // Initialize the button.
  button.begin();
  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.read();
  digitalWrite(LED_OVERRIDE, overrideBehaviour);
}
