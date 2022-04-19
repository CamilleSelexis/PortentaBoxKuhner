//Sketch to control the Magnets using the portenta in the electronics box, send the order by ethernet to the portenta
#include "Arduino.h"
#include "PCF8575.h"

#include <Portenta_Ethernet.h>
#include <Ethernet.h>
#include <math.h>
#include <stdio.h>

using namespace rtos;

#include <stdint.h>

#define Magnet0 pcf8575.digitalWrite(P0,HIGH);delay(5000);pcf8575.digitalWrite(P0,LOW);
#define Magnet1 pcf8575.digitalWrite(P1,HIGH);delay(5000);pcf8575.digitalWrite(P1,LOW);
#define Magnet2 pcf8575.digitalWrite(P2,HIGH);delay(5000);pcf8575.digitalWrite(P2,LOW);
#define Magnet3 pcf8575.digitalWrite(P3,HIGH);delay(5000);pcf8575.digitalWrite(P3,LOW);
#define Magnet4 pcf8575.digitalWrite(P4,HIGH);delay(5000);pcf8575.digitalWrite(P4,LOW);
#define Magnet5 pcf8575.digitalWrite(P5,HIGH);delay(5000);pcf8575.digitalWrite(P5,LOW);
#define Magnet6 pcf8575.digitalWrite(P6,HIGH);delay(5000);pcf8575.digitalWrite(P6,LOW);
#define Magnet7 pcf8575.digitalWrite(P7,HIGH);delay(5000);pcf8575.digitalWrite(P7,LOW);
const int LON = LOW; // Voltage level is inverted for the LED
const int LOFF = HIGH;
const int baud = 115200;

 //Ethernet related ---------------------
byte mac[] = {0xDE, 0xA1, 0x00, 0x73, 0x24, 0x12};  //Mac adress

IPAddress ip(10,0,16,10);   //Adresse IP

EthernetServer server = EthernetServer(52);  // (port 80 is default for HTTP) 52 is the number of the lab
// Uses default I2C pins -> SDA     SCL
//                Mega        20      21
//                Nano        23      24
//                Portenta    11      12

// Set i2c address
PCF8575 pcf8575(0x20);
//PCF8575 pcf8575(0x24);

void setup()
{
	Serial.begin(baud);
  digitalWrite(LEDG,LON);
	// Set pinMode to OUTPUT
  while(!Serial);
  Serial.println("Initialize the pcf8575");
  pcf8575.begin();
	pcf8575.pinMode(P0,OUTPUT);
  pcf8575.pinMode(P1,OUTPUT);
  pcf8575.pinMode(P2,OUTPUT);
  pcf8575.pinMode(P3,OUTPUT);
  pcf8575.pinMode(P4,OUTPUT);
  pcf8575.pinMode(P5,OUTPUT);
  pcf8575.pinMode(P6,OUTPUT);
  pcf8575.pinMode(P7,OUTPUT);
  

  pcf8575.digitalWrite(P0,LOW);
  pcf8575.digitalWrite(P1,LOW);
  pcf8575.digitalWrite(P2,LOW);
  pcf8575.digitalWrite(P3,LOW);
  pcf8575.digitalWrite(P4,LOW);
  pcf8575.digitalWrite(P5,LOW);
  pcf8575.digitalWrite(P6,LOW);
  pcf8575.digitalWrite(P7,LOW);
  Serial.println("Initialize the ethernet connection");
  //Ethernet setup
  Ethernet.begin(mac,ip);  //Start the Ethernet coms

// Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1500); // do nothing, no point running without Ethernet hardware
      Serial.println("No ethernet shield connected");
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }


// Start the server
  server.begin();           //"server" is the name of the object for comunication through ethernet
  Serial.print("Ethernet server connected. Server is at ");
  Serial.println(Ethernet.localIP());         //Gives the local IP through serial com
  digitalWrite(LEDB,LON);
}

void loop()
{
   // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    String currentLine = "";
    while (client.connected()) {
      if(client.available()){
        char c = client.read();
        
        if(c == '\n'){
        if(currentLine.length() !=0){
          Serial.println(currentLine);
          currentLine = "";}
        } else if(c != '\r'){
          currentLine += c;
        }
        if(currentLine.endsWith("Magnet0")){
          Magnet0;
          Serial.println("received Magnet 0 Command");
        }
        if(currentLine.endsWith("Magnet1")){
          Magnet1;
        }
        if(currentLine.endsWith("Magnet2")){
          Magnet2;
        }
        if(currentLine.endsWith("Magnet3")){
          Magnet3;
        }
        if(currentLine.endsWith("Magnet4")){
          Magnet4;
        }
        if(currentLine.endsWith("Magnet5")){
          Magnet5;
        }
        if(currentLine.endsWith("Magnet6")){
          Magnet6;
        }   
        if(currentLine.endsWith("Magnet7")){
          Magnet7;
        }            
      } 
    }     
    client.stop(); 
  } 
  //Serial.println("High");
	pcf8575.digitalWrite(P0,HIGH);
  pcf8575.digitalWrite(P1,HIGH);
  pcf8575.digitalWrite(P2,HIGH);
  pcf8575.digitalWrite(P3,HIGH);
  pcf8575.digitalWrite(P4,HIGH);
  pcf8575.digitalWrite(P5,HIGH);
  pcf8575.digitalWrite(P6,HIGH);
  pcf8575.digitalWrite(P7,HIGH);
  delay(100);
  delay(5000);
  //Serial.println("Low");
  pcf8575.digitalWrite(P0,LOW);
  pcf8575.digitalWrite(P1,LOW);
  pcf8575.digitalWrite(P2,LOW);
  pcf8575.digitalWrite(P3,LOW);
  pcf8575.digitalWrite(P4,LOW);
  pcf8575.digitalWrite(P5,LOW);
  pcf8575.digitalWrite(P6,LOW);
  pcf8575.digitalWrite(P7,LOW);
  delay(100);
	delay(5000);
}
