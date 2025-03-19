#pragma once
#include <Arduino.h>
#include "variables.h"
// #include "Arduino_LED_Matrix.h"
// ArduinoLEDMatrix matrix0;
// matrix0.begin();
// byte frame[8][12]={
//   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
// }

// --- Process Incoming Commands ---
void processCommand(WiFiClient &client) {
  //Serial.println("Taking command");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    dataIn += c;
    if (dataIn[dataIn.length()-1] == 'X'){
      // wipe data
      dataIn = "";
    }
    if (dataIn[dataIn.length()-1] == 'E'){
      // Serial.println("We're here");
      if (dataIn[0] == 'G'){
        runBuggy=true;
        //matrix.renderBitmap(frame, 8, 12);
      }
      else{
        runBuggy=false;
      }
      // disect data
      if (dataIn[1] == 'D'){
        mode = 2;
        DistanceString += dataIn[2];
        DistanceString += dataIn[3];
        DistanceString += dataIn[4];
        DistanceDesired = DistanceString.toInt();
        Serial.println();
        Serial.println(DistanceString);
        DistanceString = "";
      }
      else if (dataIn[1] == 'V'){
        mode = 1;
        RPMString += dataIn[5];
        RPMString += dataIn[6];
        RPMString += dataIn[7];
        RPMDesired = RPMString.toInt();
        Serial.println();
        Serial.println(RPMString);
        RPMString = "";
      }
      
      // Serial.print("dataIn obtained: ");
      // Serial.println(dataIn);
    }

    // Serial.println("");
    // Serial.print("Command: ");
    // Serial.println(dataIn);

    
    
  }
}