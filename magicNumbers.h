#ifndef MAGIC_NUMBERS_H
#define MAGIC_NUMBERS_H

//**** Pins ****

//Version 1
#define NFC_RST_PIN  16 
#define NFC_SS_PIN   2  
#define HALL_EFFECT_PIN   4  

//Version 2
//#define NFC_RST_PIN  2 
//#define NFC_SS_PIN   16  

#define STRIKE  15           
#define BTN     0

//#define NEOPIN     1  //Doors
#define NEOPIN     5  //Check-In  SDA

// How many NeoPixels are attached to the ESP?
#define NUMPIXELS      2

//**** Colours ****
#define OFF    0
#define RED    1  // invalid-key
#define ORANGE 2  // expiring-key
#define YELLOW 3  // Atempting to connect to Broker
#define GREEN  4  // valid-key Access Granted
#define LBLUE  5  // Start Up
#define BLUE   6  // Ready
#define PURPLE 7  // OTA Update
#define PINK   8  // Can't connect to WIFI, Resetting
#define WHITE  9  // expired-key or OTA Error


//
#define STANDALONE 1
#define CONNECTED  0

// ****** MQTT RETURN CODES ***********
#define INVALID_DEVICE 0
#define INVALID_KEY    1
#define EXPIRED_KEY    2
#define EXPIRING_KEY   3
#define VALID_KEY      4

#endif
