#pragma once
#include <Arduino.h>
#include "variables.h"

// --- Process Incoming Commands ---
void processCommand(WiFiClient &client) {
  //Serial.println("Taking command");
  while (client.available()) {
    char c = client.read();
    // Serial.print(c);
    dataIn += c;
    if (dataIn[dataIn.length()-1] == 'X'){
      // wipe data
      dataIn = "";
    }
    if (dataIn[dataIn.length()-1] == 'E'){
      // Serial.println("We're here");
      if (dataIn[0] == 'G'){
        runBuggy=true;
      }
      else{
        runBuggy=false;
      }
      // disect data
      if (dataIn[1] == 'D'){
        DistanceString += dataIn[2];
        DistanceString += dataIn[3];
        DistanceString += dataIn[4];
        DistanceDesired = DistanceString.toInt();
        // Serial.println(DistanceString);
        DistanceString = "";
      }
      else if (dataIn[1] == 'V'){
        RPMString += dataIn[5];
        RPMString += dataIn[6];
        RPMString += dataIn[7];
        RPMDesired = RPMString.toInt();
        // Serial.println(RPMString);
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