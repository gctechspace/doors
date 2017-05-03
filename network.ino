void msgRecieved(char* topic, byte* payload, unsigned int length) {
  if (DEBUG){
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      Serial.print(payload[i]);
    }
    Serial.println();
  }
  
  byte input = payload[0] -48;
  switch (input) {
    case INVALID_KEY:    
      if (DEBUG) Serial.println("INVALID-KEY");
      showColour(RED);
      break;
    case EXPIRED_KEY:   
      if (DEBUG) Serial.println("EXPIRED-KEY");
      showColour(WHITE);
      break;
    case EXPIRING_KEY:    
      if (DEBUG) Serial.println("EXPIRING-KEY");
      showColour(ORANGE);
      openDoor();
      break;
    case VALID_KEY:   
      if (DEBUG) Serial.println("VALID-KEY");
      openDoor();
      break;
  }
  
}


void connectMQTT()
{
  showColour(YELLOW);
  // Continually attemps to connect to the MQTT Broker
  while (!pubSubClient.connect(deviceName)) { //, username, password)) { 
    delay(1);
    if(DEBUG) {
      static byte cnt = 0;
      cnt++;
      if(cnt == 5) Serial.println("Connection to Broker Failed");
    }
    runStandAlone();
  }

  // Publishes an Alive message of the device name and IP address
  IPAddress myIP = WiFi.localIP();
  String aliveMsgStr = String(deviceName) + "-" + String(myIP[0]) + "." + String(myIP[1]) + "." + String(myIP[2]) + "." + String(myIP[3]);
  char aliveMsg[30];
  aliveMsgStr.toCharArray(aliveMsg, 30);
  pubSubClient.publish("alive", aliveMsg);

  // Subscribes to a topic 
  pubSubClient.subscribe(subTopic);  
}


// *****************  OTA ************************
void OTAinit()
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  
  ArduinoOTA.setHostname(deviceName);

  // No authentication by default
  //ArduinoOTA.setPassword(OTApassword);  //(const char *)

  ArduinoOTA.onStart([]() {
    //Serial.println("Start");
    showColour(PURPLE);
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    showColour(WHITE);
   /* Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); */
  });
  
  ArduinoOTA.begin();
}


// *****************  WIFI CONNECTION ************************
void connectWifi()
{
  // Continually attemps to connect to the Wifi
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    if(DEBUG) Serial.println("Connection Failed! Rebooting...");
    showColour(PINK);
    static unsigned long timeNow = millis();
    while(millis() - timeNow < resetAttempt){
      runStandAlone();
    }
    ESP.restart();
  }
}
