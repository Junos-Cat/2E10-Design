#pragma once
#include <Arduino.h>
#include "variables.h"
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

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

String returnHuskyMode(HUSKYLENSResult result);

void huskyFunction(){
  if( huskylens.request() ) {
  // Do thi ngs with the camera data
    if( !huskylens.isLearned() ){ // no learned data
      Serial.println("Nothing learned, teach me first!");
    }
    else{
      if( huskylens.available() ){
        // There are blocks to be processed, read them one by one
        while( huskylens.available() ){
          HUSKYLENSResult result = huskylens.read();
          huskyMode = returnHuskyMode(result);
        }
      }else{
        Serial.println("Nothing seen");
      }
    }
  }else{
  // Print an error
  Serial.println("huskylens.request didn't work (check connection)");
  }
}

String returnHuskyMode(HUSKYLENSResult result){
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

  return huskyMode;
  
  if (BlockWidth >30 ){ // Only change our mode if the code is close enough
    if (ID == 1){ // Turn right ahead
      return "Right";
    }
    else if (ID == 2){ // Turn left at next 
      return "Left";
    }
    else if (ID == 3){ // Go max speed
      RPMDesired = maxRPM;
      return "Forward";
    }
    else if (ID == 4){ // The speed limit is 
      RPMDesired = speedLimit1;
      return "Forward";
    }
  }
  else{ // Return the previous huskyMode i.e. don't change huskyMode
    return huskyMode;
  }
}














