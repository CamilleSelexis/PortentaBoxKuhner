//Sketch to control the Magnets using the portenta in the electronics box, send the order by ethernet to the portenta
#include "Arduino.h"
#include "PCF8575.h"
#include "MFRC522_I2C.h"
#include <Wire.h>

#include <Portenta_Ethernet.h>
#include <Ethernet.h>
#include <math.h>
#include <stdio.h>

using namespace rtos;

#include <stdint.h>

#define DEBUG 0
#define RST_PIN 13 //reset Pin for the portenta
#define Magnet0 pcf8575.pinMode(P0,OUTPUT);pcf8575.digitalWrite(P0,HIGH);delay(5000);pcf8575.digitalWrite(P0,LOW);
#define Magnet1 pcf8575.pinMode(P1,OUTPUT);pcf8575.digitalWrite(P1,HIGH);delay(5000);pcf8575.digitalWrite(P1,LOW);
#define Magnet2 pcf8575.pinMode(P2,OUTPUT);pcf8575.digitalWrite(P2,HIGH);delay(5000);pcf8575.digitalWrite(P2,LOW);
#define Magnet3 pcf8575.pinMode(P3,OUTPUT);pcf8575.digitalWrite(P3,HIGH);delay(5000);pcf8575.digitalWrite(P3,LOW);
#define Magnet4 pcf8575.pinMode(P4,OUTPUT);pcf8575.digitalWrite(P4,HIGH);delay(5000);pcf8575.digitalWrite(P4,LOW);
#define Magnet5 pcf8575.pinMode(P5,OUTPUT);pcf8575.digitalWrite(P5,HIGH);delay(5000);pcf8575.digitalWrite(P5,LOW);
#define Magnet6 pcf8575.pinMode(P6,OUTPUT);pcf8575.digitalWrite(P6,HIGH);delay(5000);pcf8575.digitalWrite(P6,LOW);
#define Magnet7 pcf8575.pinMode(P7,OUTPUT);pcf8575.digitalWrite(P7,HIGH);delay(5000);pcf8575.digitalWrite(P7,LOW);

MFRC522_I2C mfrc522[40] = {
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  MFRC522_I2C(0x00,RST_PIN),
  };
const int LON = LOW; // Voltage level is inverted for the LED
const int LOFF = HIGH;
const int baud = 115200;
int addresses[128];
int cc = 0; //Current Chip - Selects the RFID chip
bool pcf = false; //bool to know if a pcf chip is connected
bool rfid = false; //bool to know if a rfid chip is connected
int nRFID = 0; //Number of RFID chip
 //Ethernet related ---------------------
byte mac[] = {0xDE, 0x03, 0x33, 0x13, 0x59, 0x99};  //Mac adress

IPAddress ip(10,0,16,11);   //Adresse IP

EthernetServer server = EthernetServer(52);  // (port 80 is default for HTTP) 52 is the number of the lab
// Uses default I2C pins -> SDA     SCL
//                Mega        20      21
//                Nano        23      24
//                Portenta    11      12

// Set i2c address
int adr_pcf = 0x20;
PCF8575 pcf8575(adr_pcf);
//The portenta seems to have an I2C at adr 24 ??
//PCF8575 pcf8575(0x24);

void setup()
{
  Serial.begin(baud);
  while(!Serial); //Wait for the user to open the serial terminal
  digitalWrite(LEDG,LON);
  pinMode(RST_PIN,OUTPUT);
  digitalWrite(RST_PIN,LOW);
  delay(100);
  digitalWrite(RST_PIN,HIGH);
  Wire.begin();//Start the I2C communications;
  int nDevices;
  nDevices = scan_i2c(addresses);
  //MFRC522_I2C mfrc522[nDevices] = {0}; //Create the array to contain the instances of MFRC522
  for(int i = 0; i<nDevices; i++){ //Create instances of pcf and rfid depending on what was found on i2c bus
    if(addresses[i] == adr_pcf){
      Serial.println("PCF8575 connected to the I2C Bus");
      Serial.println("Initialize the pcf8575");
      init_pcf8575(adr_pcf); //init the pcf8575 with its adr
      pcf = true;
    }
    else if(addresses[i] == 0x24){
      Serial.println("0x24 adress used");
    }
    else{
      Serial.print("MFRC522 detected at adr ");Serial.println(addresses[i]);
      mfrc522[cc] = MFRC522_I2C(addresses[i],RST_PIN);
      mfrc522[cc].PCD_Init();
      ShowReaderDetails(cc);
      nRFID++;
      cc++;
      rfid  = true;
    }
  }
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
  server.begin();           //"s33+-+----------------erver" is the name of the object for comunication through ethernet
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
        uint8_t data = client.read(); //read first byte of data
        
        if(data == 0X00){
          //Command for the PCF8575
          if(pcf){//Check that a pcf8575 is connected
            data = client.read();
            Serial.print("Received data : ");Serial.println(data,HEX);
            switch(data){
              case 0x00: 
                Magnet0;
              break;
              case 0x01:
                Magnet1;
                break;
              case 0x02:
                Magnet2;
                break;
              case 0x03:
                Magnet3;
                break;
              case 0x04:
                Magnet4;
                break;
              case 0x05:
                Magnet5;
                break;
              case 0x06:
                Magnet6;
                break;
              case 0x07:
                Magnet7;
                break;
              default:
                Serial.println("No such instruction");
                break;
            }
          }
          else{
            server.write("No PCF connected");
          }
        }
        else if(data == 0xFF){
          //Command for the RFID
          //0x FF chip# R/W msg
          Serial.println("RFID instr. received.");
          if(rfid){//Check that a mfrc522 chip is connected
            data = client.read();
            if(data<nRFID){
              cc = data; //Select the correct chip
              Serial.print("Chip ");Serial.print(cc);Serial.println("selected");
                if (mfrc522[cc].PICC_IsNewCardPresent()&&mfrc522[cc].PICC_ReadCardSerial()) {
                  server.write("RFID tag found");
                  // Dump UID
                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522[cc].uid.size; i++) {
                    Serial.print(mfrc522[cc].uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(mfrc522[cc].uid.uidByte[i], HEX);
                  } 
                  Serial.println();
                  // Show the whole sector as it currently is
                  Serial.println(F("Current data in Tag:"));
                  mfrc522[cc].PICC_DumpMifareUltralightToSerial();
                  Serial.println();
                  data = client.read();//read next byte
                  if(data == 0xFF){ //Will perform a write on the tag
                    byte pageAddr      = 2;
                    byte dataBlock[] = {0x00,0x00,0x00,0x00};
                    byte buffer[18];
                    byte size = sizeof(buffer);
                    int success = 0;
                    MFRC522_I2C::StatusCode status;
                    for(byte pageAddr = 5;pageAddr<8;pageAddr++){
                      Serial.print(F("Writing data into page ")); Serial.print(pageAddr);
                      Serial.println(F(" ..."));
                      for(int i = 0; i<4; i++){
                        dataBlock[i] = client.read();
                      }
                      dump_byte_array(dataBlock, 4); Serial.println();
                      status = (MFRC522_I2C::StatusCode) mfrc522[cc].MIFARE_Ultralight_Write(pageAddr, dataBlock, 4);
                      if (status != MFRC522_I2C::STATUS_OK) {
                          Serial.print(F("MIFARE_Write() failed: "));
                          Serial.println(mfrc522[cc].GetStatusCodeName(status));
                      }
                      Serial.println();

                      // Read data from the block (again, should now be what we have written)
                      Serial.print(F("Reading data from page ")); Serial.print(pageAddr);
                      Serial.println(F(" ..."));
                      status = (MFRC522_I2C::StatusCode) mfrc522[cc].MIFARE_Read(pageAddr, buffer, &size);
                      if (status != MFRC522_I2C::STATUS_OK) {
                          Serial.print(F("MIFARE_Read() failed: "));
                          Serial.println(mfrc522[cc].GetStatusCodeName(status));
                      }
                      Serial.print(F("Data in page ")); Serial.print(pageAddr); Serial.println(F(":"));
                      dump_byte_array(buffer, 4); Serial.println();
                  
                      // Check that data in block is what we have written
                      // by counting the number of bytes that are equal
                      Serial.println(F("Checking result..."));
                      byte count = 0;
                      for (byte i = 0; i < 4; i++) {
                          // Compare buffer (= what we've read) with dataBlock (= what we've written)
                          if (buffer[i] == dataBlock[i])
                              count++;
                      }
                      Serial.print(F("Number of bytes that match = ")); Serial.println(count);
                      if (count == 4) {
                          Serial.println(F("Success :-)"));
                          success++;
                      } else {
                          Serial.println(F("Failure, no match :-("));
                          Serial.println(F("  perhaps the write didn't work properly..."));
                      }
                      Serial.println();
                    }
                    if(success == 3){
                      server.write("Tag has been updated");
                    }
                    else{
                      server.write("Error writing on the tag");
                    }
                  }
                  mfrc522[cc].PCD_StopCrypto1();
                }
                else{Serial.println("No tag found");}
            }
          }
          else{
            server.write("No RFID connected");
          }
        }       
      } 
    }     
    client.stop(); 
  }
  digitalWrite(LEDG,LOW);
  delay(100);
  digitalWrite(LEDG,HIGH);
  delay(100);
}
