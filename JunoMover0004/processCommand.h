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
    if (c == 'E'){
      // wipe data
      data = "";
    }
    if (c == 'X'){
      // disect data
      splitData = split(data, ",");
      if (splitData[1] == "D"){
        //desiredDistance = float(splitData[4])
      }
      else if (splitData[1] == "V"){
        //desiredRPM = float(splitData[3])
      }
      if (splitData[0] == 'G'){
        runBuggy=true;
      }
      else{
        runBuggy=false;
      }
      Serial.print("Data obtained: ");
      Serial.println(data);
    }
    Serial.println("");
    Serial.print("Command: ");
    Serial.println(data);

    
    
  }
}