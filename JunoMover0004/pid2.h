#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){
  left_motor_move(leftRPMDesired, Vmax);
  right_motor_move(rightRPMDesired, Vmax);
}
