#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

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



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
