#pragma once
#include <Arduino.h>
#include "variables.h"

// Encoder interrupt functions
void left_encoder_interrupt(){
  leftEncoderCount ++;
}

void right_encoder_interrupt(){
  rightEncoderCount ++;
}