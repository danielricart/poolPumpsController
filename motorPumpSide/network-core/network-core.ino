//opta core M4 code
//M7 core does the main reading / output(controls input, output relays and status leds)
//M4 does the networking and modbus.controls the RESET LED Red / Green
#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif
#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include "RPC.h"  // comes with the mbed board installation


#define LED_CONN_ERROR LEDR
#define LED_CONN_OK LEDG

#define ADDR_REMOTE_MOTOR 0x00
#define ADDR_REMOTE_LED_WATER_LEVEL 0x01
#define ADDR_REMOTE_LED_CHLORINE 0x02


IPAddress ip(192, 168, 1, 177);

EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);


IPAddress server(192, 168, 1, 10);  // update with the IP Address of your Modbus server

int waterLevel = LOW;
int chlorineStatus = LOW;
int remoteMotorStatus = LOW;
int turnMotorOn = LOW;

bool etherStatus = false;
bool remoteComms = false;

void setWaterLevelStatus(int newStatus) {
  waterLevel = newStatus;
}

void setChlorineStatus(int newStatus) {
  chlorineStatus = newStatus;
}

void setTurnMotorOn(int newStatus) {
  turnMotorOn = newStatus;
}

int getRemoteMotorStatus() {
  return remoteMotorStatus;
}

void setup() {
  // put your setup code here, to run once:

  Ethernet.begin(ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      digitalWrite(LED_CONN_ERROR, HIGH);
      delay(50);  // do nothing, no point running without Ethernet hardware
      digitalWrite(LED_CONN_ERROR, LOW);
      delay(50);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  RPC.begin();
  RPC.bind("setWaterLevelStatus", setWaterLevelStatus);
  RPC.bind("setChlorineStatus", setChlorineStatus);
  RPC.bind("setTurnMotorOn", setTurnMotorOn);
  RPC.bind("getRemoteMotorStatus", getRemoteMotorStatus);

  bootM4();
}

void loop() {
  // put your main code here, to run repeatedly:
  // READ INPUTS


  // ACT


  // COMMS
  if (!modbusTCPClient.connected()) {
    // client not connected, start the Modbus TCP client
    Serial.println("Attempting to connect to Modbus TCP server");

    if (!modbusTCPClient.begin(server, 502)) {
      Serial.println("Modbus TCP Client failed to connect!");
      remoteComms = false;
    } else {
      Serial.println("Modbus TCP Client connected");
      remoteComms = true;
    }
  } else {  // client connected
    remoteComms = modbusTCPClient.connected();

    int remoteInput1 = modbusTCPClient.coilRead(0x00);
    if (remoteInput1 < 0) {
      Serial.print("Failed to read input! ");
      Serial.println(modbusTCPClient.lastError());
      remoteComms = false;
    } else {
      remoteMotorStatus = remoteInput1;
    }

    if (!modbusTCPClient.coilWrite(ADDR_REMOTE_MOTOR, turnMotorOn)) {
      Serial.print("Failed to send turnMotorOn! ");
      Serial.println(modbusTCPClient.lastError());
      remoteComms = false;
    }
    if (!modbusTCPClient.coilWrite(ADDR_REMOTE_LED_WATER_LEVEL, waterLevel)) {
      Serial.print("Failed to send waterLevel! ");
      Serial.println(modbusTCPClient.lastError());
      remoteComms = false;
    }
    if (!modbusTCPClient.coilWrite(ADDR_REMOTE_LED_CHLORINE, chlorineStatus)) {
      Serial.print("Failed to send chlorineStatus! ");
      Serial.println(modbusTCPClient.lastError());
      remoteComms = false;
    }
  }

  // OUTPUTS
  etherStatus = (Ethernet.linkStatus() == LinkON);
  if (etherStatus && remoteComms) {
    digitalWrite(LED_CONN_OK, HIGH);
    digitalWrite(LED_CONN_ERROR, LOW);
  } else if (etherStatus && !remoteComms) {
    digitalWrite(LED_CONN_OK, HIGH);
    digitalWrite(LED_CONN_ERROR, HIGH);
  } else {
    digitalWrite(LED_CONN_OK, LOW);
    digitalWrite(LED_CONN_ERROR, HIGH);
  }
}
