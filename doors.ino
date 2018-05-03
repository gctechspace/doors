//****************************** GCTS Door opener ******************************
/*
 *                   Uses and ESP8266 and a RC522 NFC reader
 *    Transmits the door's ID and the RFID code to a local server via MQTT
 *    
 *    PCB & ESP Code by Skip Christian
 *    
 *    Copy credentials.example to credentials.h and fill in your details
 */
 
#define DEBUG 0

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>

#include "credentials.h"
#include "magicNumbers.h"

unsigned long resetAttempt = 300000; // 5mins
boolean doorOpen = false;
long openTime = 0;
int duration = 4000;   //This should be set upon startup

union tagID {
   unsigned long value;
   byte bytes[4];
};
tagID recievedID;

unsigned long newTag;

// ************ Function Prototypes ***************
void houseKeeping();
void openDoor();
void closeDoor();
void showColour(byte);
unsigned long getID();
void checkID(boolean);
void msgRecieved(char* topic, byte* payload, unsigned int length);
void connectMQTT();
void OTAinit();
void connectWifi();


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

MFRC522 mfrc522(NFC_SS_PIN, NFC_RST_PIN); // Create MFRC522 instance

WiFiClient wifiClient;
PubSubClient pubSubClient(MQTT_SERVER, MQTT_PORT, msgRecieved, wifiClient);

volatile boolean doorStatePrevious = 0; // 0=closed, 1=open // todo: move to magic numbers.
volatile boolean doorStateCurrent = 0; // 0=closed, 1=open // todo: move to magic numbers.
volatile boolean doorStateChange = 0; 
volatile unsigned long intTime = 0;
#define INT_DEBOUNCE 100

void handleDoorStateChangeISR()
{
  doorStateCurrent = digitalRead(HALL_EFFECT_PIN) ? 1 : 0;  // 0=closed, 1=open // todo: move to magic numbers.
  unsigned long timeNow = millis();
  if(doorStateCurrent != doorStatePrevious || timeNow - intTime > INT_DEBOUNCE){
    doorStateChange = 1; // Tell our main loop that the door state change needs to be sent via MQTT
    intTime = timeNow;
  }
}

void setup() 
{
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522 library. 
  
  if(DEBUG) {
    Serial.begin(115200);
    Serial.println("Hi");
  } else {
    pixels.begin();        // Init NeoPixel library.
  }
  sprintf(SUB_TOPIC,"%s%s%s",PUB_TOPIC,"/",DEVICE_NAME);
  pinMode(STRIKE, OUTPUT);
  pinMode(HALL_EFFECT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(HALL_EFFECT_PIN), handleDoorStateChangeISR, CHANGE);

  showColour(LBLUE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  connectWifi();

  OTAinit();

  connectMQTT();
  
  showColour(BLUE); 
}



void loop() 
{
  newTag = getID();  // Checks for a RFID tag
  if(newTag) checkID(CONNECTED);
  
  //If the door has been open for the set duration, close it
  if(doorOpen == true && millis() - openTime > duration) {
    closeDoor();
    if(DEBUG) {
      Serial.println("Door Closed");
      delay(20);
    }
  }

  //If the button is pressed, open the door
  if(digitalRead(BTN) == LOW){
    openDoor();
    if(DEBUG) {
      Serial.println("Door Open");
      delay(20);
    }
  } 

  if(doorStateChange){
    doorStateChange = 0;
    String doorMsgStr;
    if(digitalRead(HALL_EFFECT_PIN)){
      doorMsgStr = String(DEVICE_NAME) + ";open";
    } else {
      doorMsgStr = String(DEVICE_NAME) + ";closed";
    }
    char msg[20];
    doorMsgStr.toCharArray(msg, 20);
    pubSubClient.publish("techspace/doors", msg);
  }

  houseKeeping();
}

void houseKeeping()
{
  //*************** House Keeping *******************
  //If we drop off Wifi, atempt to reconnect
  if (WiFi.status() != WL_CONNECTED) {
    if(DEBUG) Serial.println("Lost Wifi, Reconnecting");
    connectWifi();
  }
  //If we drop off Broker, try connecting again
  if (!pubSubClient.connected()) {
    if(DEBUG) Serial.println("Lost Broker, Reconnecting");
    connectMQTT();
  } 
  
  pubSubClient.loop();
  ArduinoOTA.handle();
}

void openDoor() {
  doorOpen = true;
  openTime = millis();
  digitalWrite(STRIKE, HIGH);
  showColour(GREEN);
}

void closeDoor() {
  doorOpen = false;
  digitalWrite(STRIKE, LOW);
  showColour(BLUE);
}

