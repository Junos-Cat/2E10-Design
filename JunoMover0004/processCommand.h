#pragma once
#include <Arduino.h>

// --- Process Incoming Commands ---
void processCommand(WiFiClient &client) {
  while (client.available()) {
    char c = client.read();
    if (c=='G'){
      runBuggy=true;
      // Serial.print("command: ");
      // Serial.println(c);
    }
    else{
      runBuggy=false;
      // Serial.print("command: ");
      // Serial.println(c);
    }
  }
}