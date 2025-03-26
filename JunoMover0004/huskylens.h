#pragma once
#include <Arduino.h>
#include "variables.h"
#include "HUSKYLENS.h"

void setup(){
  Serial.begin(9600);
  Wire.begin();
    while( !huskylens.begin(Wire) ){
    Serial.println("Oh no! Check HuskyLens connection");
    delay(1000);
  }
}

oid loop(){
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

String returnHuskyMode(result){
  CenterXPos = result.xCenter;
  CenterYPos = result.yCenter;
  BlockWdith = result.width;
  BlockHeight = result.height;
  ID = result.ID;
  
  if (BlockWidth > ){ // Only change our mode if the code is close enough
    if (ID == ){ // Turn right ahead
      return "Right";
    }
    else if (ID == ){ // Turn left at next 
      return "Left";
    }
  }
  else{ // Return the previous huskyMode i.e. don't change huskyMode
    return huskyMode;
  }
}














