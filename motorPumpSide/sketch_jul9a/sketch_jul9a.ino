/* 
Connections:
I1: Pump Sensor
RELAY1: Motor Driver

LEDs:
RESET: Green-> Connection established to remote board
RESET: Red -> Cannot connect to remote board

STATUS_1: Pump is read ON
STATUS_2: Motor is ON
STATUS_3: NC
STATUS_4: Overriden. Pump is assumed always ON. Pump Input is ignored

Buttons:
USER: Long press (2 seconds) toggles OVERRIDE. Check LED4.

Behaviour:
Automatic (default mode):

- When I1 is HIGH for MOTOR_DELAY_ON milliseconds, it sets HIGH RELAY OUTPUT 1.
- When I1 is LOW for MOTOR_DELAY_OFF milliseconds, it sets LOW RELAY OUTPUT 1.



*/

#include <avdweb_VirtualDelay.h>
#include <EasyButton.h>

#define PUMP_SIGNAL PIN_A1
#define PUMP_SIGNAL_LED LED_D0
#define MOTOR_LED LED_D1
#define MOTOR_PIN RELAY2

#define OVERRIDE_LED LED_D3

#define MOTOR_DELAY_ON 10000  // milliseconds to wait between PUMP_SIGNAL == HIGH and MOTOR_PIN = HIGH
#define MOTOR_DELAY_OFF 5000  // milliseconds to wait between PUMP_SIGNAL==LOW and MOTOR_PIN = LOW

#define LED_CONN_ERROR LEDR
#define LED_CONN_OK LEDG

#define BUTTON_LONG_PRESS 2000

// #define LED_REMOTE_CONNECTED shared led.
// LED_RESET --> LEDG --> green above reset
// LEDR --> red above reset. shared with GREEN.
// LEDR + LEDG ==> orange led

//LED_D0, LED_D1, LED_D2, LED_D3
// BTN_USER
EasyButton button(BTN_USER);


// Potentially managed in core M4
bool connStatus = false;
bool overrideBehaviour = false;

bool pumpStatus = LOW;
bool motorStatus = LOW;

//noDelay delayMotorOn(10000, turnMotorOn, false);//Creats a noDelay varible set to 1000ms, will call ledBlink function
//noDelay delayMotorOff(10000, turnMotorOff, false);
VirtualDelay delayMotorOn;
VirtualDelay delayMotorOff;

void onPressedForDuration() {
  overrideBehaviour = !overrideBehaviour;
  pumpStatus = !pumpStatus;
  if (overrideBehaviour) {
    Serial.print("Behaviour is overridden. going manual. ");
    Serial.print("pumpStatus is ");
    Serial.println(pumpStatus);
  } else
    Serial.println("Behaviour is automatic");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PUMP_SIGNAL, INPUT_PULLUP);
  pinMode(PUMP_SIGNAL_LED, OUTPUT);
  pinMode(MOTOR_LED, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  pinMode(LED_CONN_OK, OUTPUT);
  pinMode(LED_CONN_ERROR, OUTPUT);


  digitalWrite(PUMP_SIGNAL_LED, digitalRead(PUMP_SIGNAL));
  digitalWrite(MOTOR_LED, LOW);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(LED_CONN_OK, LOW);
  digitalWrite(LED_CONN_ERROR, LOW);


  // Initialize the button.
  button.begin();

  // Attach callback.
  button.onPressedFor(BUTTON_LONG_PRESS, onPressedForDuration);
}

void loop() {
  // put your main code here, to run repeatedly:
  // READ INPUTS
  button.read();  //Btn is read first because it may block the next reading or not.

  if (!overrideBehaviour) {
    pumpStatus = digitalRead(PUMP_SIGNAL);
  }


  // ACT
  if (delayMotorOn.elapsed()) {
    motorStatus = pumpStatus;
    Serial.print("Wait time to turn on motor past. Motor is ");
    Serial.println(motorStatus);
  }
  if (delayMotorOff.elapsed()) {
    motorStatus = pumpStatus;
    Serial.print("Wait time to turn off motor past. Motor is ");
    Serial.println(motorStatus);
  }


  if (pumpStatus) {
    delayMotorOn.start(MOTOR_DELAY_ON);
    delayMotorOff.running = false;
    //Serial.println("Pump is ON");
  } else {
    delayMotorOn.running = false;
    delayMotorOff.start(MOTOR_DELAY_OFF);
    //Serial.println("Pump is OFF");
  }

  // COMMUNICATE



  // UPDATE OUTPUTS
  //TODO: UPDATE connStatus
  digitalWrite(LED_CONN_ERROR, !connStatus);
  digitalWrite(LED_CONN_OK, connStatus);

  digitalWrite(PUMP_SIGNAL_LED, pumpStatus);
  digitalWrite(MOTOR_LED, motorStatus);

  digitalWrite(OVERRIDE_LED, overrideBehaviour);
}
