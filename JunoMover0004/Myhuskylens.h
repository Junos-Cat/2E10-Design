#pragma once
#include <Arduino.h>
#include "variables.h"
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"


// myHuskySetup fonksiyonu
void myHuskySetup(){
  Serial.begin(9600);
  Wire.begin();
  while( !huskylens.begin(Wire) ){
    Serial.println("Oh no! Check HuskyLens connection");
    delay(1000);
  }
  if( huskylens.begin(Wire) ){
    Serial.println("HuskyLens connected");
  }
}

// Fonksiyon prototipi
String returnHuskyMode(HUSKYLENSResult result);

// huskyFunction: Kamera verilerini işler
void huskyFunction(){
  if( huskylens.request() ) {
    // Kamera verileriyle işlem yap
    if( !huskylens.isLearned() ){ // Öğrenilmemiş data varsa
      Serial.println("Nothing learned, teach me first!");
    }
    else{
      if( huskylens.available() ){
        // Blokları tek tek oku
        while( huskylens.available() ){
          HUSKYLENSResult result = huskylens.read();
          huskyMode = returnHuskyMode(result);
        }
      } else {
        Serial.println("Nothing seen");
      }
    }
  } else {
    // Hata mesajı yazdır
    Serial.println("huskylens.request didn't work (check connection)");
  }
}

// returnHuskyMode: HuskyLens'ten gelen veriye göre mod kararını verir
String returnHuskyMode(HUSKYLENSResult result) {
  // Assign received tag data to global variables
  CenterXPos = result.xCenter;
  CenterYPos = result.yCenter;
  BlockWidth = result.width;
  BlockHeight = result.height;
  ID = result.ID;

  Serial.println(result.xCenter);
  Serial.println(result.yCenter);
  Serial.println(result.width);
  Serial.println(result.height);
  Serial.println(result.ID);

  // If the tag is large enough (i.e., close enough), process it:
  if (BlockWidth > 25) {
    if (ID == 1) {  // Turn right ahead
      activeTagCommand = "Right";
      return activeTagCommand;
    }
    else if (ID == 2) {  // Turn left at next junction
      activeTagCommand = "Left";
      return activeTagCommand;
    }
    else if (ID == 5) {  // Go max speed command
      if (tagDetectedTime == 0) {
        tagDetectedTime = millis(); // Record the time when tag is first detected
        tagDetectionTravelDistance = averageTravelDistance; // Record current travel distance
      }
      // If the buggy has traveled at least 15 cm since detecting the tag
      if ((averageTravelDistance - tagDetectionTravelDistance) >= 15) {
         RPMDesired = maxRPM;
         activeTagCommand = "Forward"; // Set the command to "Forward"
         // Reset detection variables for next tag
         tagDetectedTime = 0;
         tagDetectionTravelDistance = 0;
         return activeTagCommand;
      } else {
         // Until 20 cm is traversed, return the already active command if available,
         // otherwise, return the current mode.
         return (activeTagCommand != "") ? activeTagCommand : huskyMode;
      }
    }
    else if (ID == 7) {  // Speed limit command
      if (tagDetectedTime == 0) {
        tagDetectedTime = millis();
        tagDetectionTravelDistance = averageTravelDistance;
      }
      if ((averageTravelDistance - tagDetectionTravelDistance) >= 20) {
         RPMDesired = speedLimit1;
         activeTagCommand = "Forward";
         tagDetectedTime = 0;
         tagDetectionTravelDistance = 0;
         return activeTagCommand;
      } else {
         return (activeTagCommand != "") ? activeTagCommand : huskyMode;
      }
    }
  }
  // If no valid tag is detected, return the active command if it exists; otherwise return the current huskyMode.
  return (activeTagCommand != "") ? activeTagCommand : huskyMode;
}
