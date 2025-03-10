#pragma once
#include <Arduino.h>

void pid(sideMotor, speed){
  analogueWrite(sideMotor, speed);
}