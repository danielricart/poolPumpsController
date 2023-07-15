#ifdef CORE_CM7
#error "This Code is for CORE_CM4 only"
#endif

#include <EasyButton.h>

#define BUTTON_LONG_PRESS 2000
#define LED_OVERRIDE LED_D3

#define PIN_WATERLEVEL A0
#define LED_WATERLEVEL LED_D0

EasyButton button(BTN_USER);
bool overrideBehaviour = false;

bool waterLevelStatus = false;
bool turnRemoteMotorOn = false;

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_OVERRIDE, OUTPUT);
  digitalWrite(LED_OVERRIDE, LOW);

  pinMode(LED_WATERLEVEL, OUTPUT);
  pinMode(PIN_WATERLEVEL, INPUT_PULLDOWN);

  // Initialize the button.
  button.begin();
  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);
}

void loop() {
  // READ INPUTS
  button.read();
  waterLevelStatus = digitalRead(PIN_WATERLEVEL);
  //TODO: Read RemoteMotorStatus

  // EVALUATE
  turnRemoteMotorOn = waterLevelStatus;


  // WRITE OUTPUTS
  digitalWrite(LED_WATERLEVEL, waterLevelStatus);
  digitalWrite(LED_OVERRIDE, overrideBehaviour);

  //TODO: RPC turnRemoteMotorOn
}
