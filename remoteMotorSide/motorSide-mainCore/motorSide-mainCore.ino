// MotorSide Main Core.
#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif

#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include <avdweb_VirtualDelay.h>
#include "RPC.h"

#define LED_CONN_ERROR LEDR
#define LED_CONN_OK LEDG

#define ADDR_MOTOR 0x00
#define ADDR_MOTOR_STATUS 0x00
#define ADDR_REMOTE_LED_WATER_LEVEL 0x01
#define ADDR_REMOTE_LED_CHLORINE 0x02

IPAddress ip(192, 168, 1, 10);
EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;

VirtualDelay updateSerialOutput;

int remoteWaterLevel = LOW;
int remoteChlorineStatus = LOW;
int MotorStatus = LOW;
int turnMotorOn = LOW;

bool overrideBehaviour = false;

bool modbusStatus = false;
bool remoteComms = false;

int getRemoteWaterLevel() {
  return remoteWaterLevel;
}

int getRemoteChlorineStatus() {
  return remoteChlorineStatus;
}

int getTurnMotorOn() {
  return turnMotorOn;
}

void setMotorStatus(int newValue) {
  MotorStatus = (bool)newValue;
}

void setup() {
  // put your setup code here, to run once:
  bootM4();
  updateSerialOutput.start(1000);

  RPC.begin();
  RPC.bind("getRemoteWaterLevel", getRemoteWaterLevel);
  RPC.bind("getRemoteChlorineStatus", getRemoteChlorineStatus);
  RPC.bind("getTurnMotorOn", getTurnMotorOn);
  RPC.bind("setMotorStatus", setMotorStatus);
  delay(200);

  Ethernet.begin(ip);
  if (Ethernet.linkStatus() == LinkOFF) {
    //Serial.println("Ethernet cable is not connected.");
    delay(500);
  }
  ethServer.begin();

  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1)
      ;
  }

  if (modbusTCPServer.configureCoils(0x00, 3) != 1)
    Serial.println("Error configuring 3 coils");  //TurnOn Motor (maps to a relay)

  // this is an discrete input
  if (modbusTCPServer.configureDiscreteInputs(0x00, 1) != 1)  //Motor status (maps to an input)
    Serial.println("Error configuring 1 discrete input");
  else
    Serial.println("MotorPump Controller Started.");
}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = ethServer.available();

  if (client) {
    modbusTCPServer.accept(client);
    modbusStatus = true;
    while (client.connected() && Ethernet.linkStatus() == LinkON) {
      // poll for Modbus TCP requests, while client connected
      modbusTCPServer.poll();
      modbusStatus = true;
      remoteComms = true;
      readModbus();
      updateStatus();
      updateSerial();
    }
    //client disconnected
    Serial.println("MODBUS client disconnected.");
  } else {
    modbusStatus = false;
    remoteComms = false;
    updateStatus();
    updateSerial();
    // Disable outputs if there's no connection. Safe mode. 
    turnMotorOn = false;
    remoteWaterLevel = false;
    remoteChlorineStatus = false;
    Serial.println("No MODBUS Client found .");
  }
}

void readModbus() {
  turnMotorOn = (bool)modbusTCPServer.coilRead(ADDR_MOTOR);
  modbusTCPServer.discreteInputWrite(ADDR_MOTOR_STATUS, MotorStatus);
  remoteWaterLevel = (bool)modbusTCPServer.coilRead(ADDR_REMOTE_LED_WATER_LEVEL);
  remoteChlorineStatus = (bool)modbusTCPServer.coilRead(ADDR_REMOTE_LED_CHLORINE);
}

void updateStatus() {



  digitalWrite(LEDR, !modbusStatus);
  digitalWrite(LEDG, modbusStatus);
}

void updateSerial() {
  if (updateSerialOutput.elapsed()) {
    Serial.print("modbusStatus: ");
    Serial.print(modbusStatus);
    Serial.print(" remoteWaterLevel: ");
    Serial.print(remoteWaterLevel);
    Serial.print(" MotorStatus: ");
    Serial.print(MotorStatus);
    Serial.print(" remoteChlorineStatus: ");
    Serial.print(remoteChlorineStatus);
    Serial.print(" turnMotorOn: ");
    Serial.print(turnMotorOn);
    Serial.println(" ");
    updateSerialOutput.start(1000);
  }
}
