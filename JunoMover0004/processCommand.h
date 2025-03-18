#pragma once
#include <Arduino.h>
#include "variables.h"

// --- Process Incoming Commands ---
void processCommand(WiFiClient &client) {
  //Serial.println("Taking command");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    data += c;
    if (data[data.length()-1] == 'X'){
      // wipe data
      data = "";
    }
    if (data[data.length()-1] == 'E'){
      Serial.println("We're here");
      if (data[0] == 'G'){
        runBuggy=true;
      }
      else{
        runBuggy=false;
      }
      // disect data
      // splitData = split(data, ",");
      if (data[1] == 'D'){
        //desiredDistance = float(splitData[4])
      }
      else if (data[1] == 'V'){
        //desiredRPM = float(splitData[3])
      }
      
      Serial.print("Data obtained: ");
      Serial.println(data);
    }

    Serial.println("");
    Serial.print("Command: ");
    Serial.println(data);

    
    
  }
}