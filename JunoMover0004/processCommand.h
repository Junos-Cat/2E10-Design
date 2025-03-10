#pragma once
#include <Arduino.h>
#include "variables.h"

// --- Process Incoming Commands ---
void processCommand(WiFiClient &client) {
  while (client.available()) {
    char c = client.read();
    if (c=='G'){
      runBuggy=true;
    }
    else{
      runBuggy=false;
    }
  }
}