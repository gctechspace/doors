void runStandAlone()
{
  newTag = getID();
  if(newTag) {
    checkID(STANDALONE);
  } else {

    //If the door has been open for the set duration, close it
    if(doorOpen == true && millis() - openTime > duration) {
      closeDoor();
      showColour(PINK);
      if(DEBUG) {
        Serial.println("Door Closed");
        delay(20);
      }
    }
  }
  
}

unsigned long getID()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return 0;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return 0;
  }
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    recievedID.bytes[i] = mfrc522.uid.uidByte[i];
  }

  if (DEBUG) Serial.println(recievedID.value);
  return recievedID.value;
}

void checkID(boolean standAlone)
{
  static unsigned long lastTag = 0;
  static unsigned long timeNow = 0;
  // debounces multiple reads
  if(millis() - timeNow > duration || lastTag != newTag){
    for(byte i = 0; i < 12; i++) {  //loop through masterTags to see if any are the same
      if(newTag == masterTags[i]) {
        openDoor();
      }
    }

    if(!standAlone){
      String msgStr = String(DEVICE_NAME) + ";" + String(newTag);
      char msg[20];
      msgStr.toCharArray(msg, 20);
      pubSubClient.publish(PUB_TOPIC, msg);
    }
    lastTag = newTag;
    timeNow = millis();
  }
}

