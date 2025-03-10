#pragma once
#include <Arduino.h>

sendMessage(){
  MessageTimeElapsed = 0;
  // Send data to the laptop client
  message = String(leftEncoderCount) + "," + String(distance) + ",";
  client.println(message);
}