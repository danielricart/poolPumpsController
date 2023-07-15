#ifdef CORE_CM4
#error "This Code is for CORE_CM7 only"
#endif

#include <avdweb_VirtualDelay.h>
#include "RPC.h"

#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>


#define LED_CONN_ERROR LEDR
#define LED_CONN_OK LEDG

#define ADDR_REMOTE_MOTOR 0x00
#define ADDR_REMOTE_LED_WATER_LEVEL 0x01
#define ADDR_REMOTE_LED_CHLORINE 0x02
IPAddress ip(192, 168, 1, 177);  //Local end

EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);
IPAddress server(192, 168, 1, 10);  // remote end

VirtualDelay updateSerialOutput;

bool waterLevel = false;
bool chlorineStatus = false;
bool motorOn = false;
bool remoteMotorStatus = false;
bool overrideBehaviour = false;

bool modbusStatus = false;
bool remoteComms = false;

void setVariables(bool newWaterLevel, bool newChlorineStatus, bool newMotorOn, bool newOverride) {
  waterLevel = newWaterLevel;
  chlorineStatus = newChlorineStatus;
  motorOn = newMotorOn;
  overrideBehaviour = newOverride;
}

int getRemoteMotorStatus() {
  return remoteMotorStatus;
}

void setup() {
  // put your setup code here, to run once:
  bootM4();
  updateSerialOutput.start(1000);
  RPC.begin();
  RPC.bind("setVariables", setVariables);
  RPC.bind("getRemoteMotorStatus", getRemoteMotorStatus);
  delay(200);

  Ethernet.begin(ip);
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!modbusTCPClient.connected()) {
    // client not connected, start the Modbus TCP client
    Serial.println("Attempting to connect to Modbus TCP server");

    if (!modbusTCPClient.begin(server, 502)) {
      Serial.println("Modbus TCP Client failed to connect!");
      modbusStatus = false;
    } else {
      Serial.println("Modbus TCP Client connected");
      modbusStatus = true;
    }
  } else {  // client connected
    modbusStatus = modbusTCPClient.connected();
    remoteComms = true; //assuming everything will be ok. 
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


  //TODO: Fetch this from remote end.
  //remoteMotorStatus = motorOn;

  digitalWrite(LEDR, !modbusStatus || !remoteComms);
  digitalWrite(LEDG, modbusStatus || remoteComms);


  if (updateSerialOutput.elapsed()) {
    Serial.print("waterLevel: ");
    Serial.print(waterLevel);
    Serial.print("  chlorineStatus: ");
    Serial.print(chlorineStatus);
    Serial.print("  motorOn: ");
    Serial.print(motorOn);
    Serial.print("  remoteMotorStatus: ");
    Serial.print(remoteMotorStatus);
    Serial.print("  overrideBehaviour: ");
    Serial.print(overrideBehaviour);

    Serial.println(" ");
    updateSerialOutput.start(1000);
  }
}
