#pragma once
#include <Arduino.h>
#include "variables.h"

// Encoder interrupt functions
void left_encoder_interrupt(){
  leftEncoderCount = leftEncoderCount + 1;
}

void right_encoder_interrupt(){
  rightEncoderCount ++;
}