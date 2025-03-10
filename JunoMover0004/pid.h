#pragma once
#include <Arduino.h>
#include "variables.h"

void pid(int sideMotor, int speed){
  analogWrite(sideMotor, speed);
  // new code
  // silver challenge code
}