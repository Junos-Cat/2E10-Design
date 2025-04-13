#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){

  // Error
  if (UIMode == 1){ // Speed control
    E = RPMDesired - (leftRPM + rightRPM)/2;
  }
  else if (UIMode == 2){ // Distance control
    E = USSensorDistance - DistanceDesired;
  }

  // Integration
  Inte = IntePrevious + (dt * (E + EPrevious) * half);

  // Differenciation
  Diff = (E - EPrevious) / dt;

  // Voltage
  V = VPrevious + (kpSpeed * E + kiSpeed * Inte + kdSpeed * Diff);


  // Prevention of antiwinder
  if (V > Vmax) {
    V = Vmax;
    Inte = IntePrevious;
  }
  if (V < Vmin) {
    V = Vmin;
    Inte = IntePrevious;
  }

  // Update voltages
  VPrevious = V;

  left_motor_move(V * leftF, Vmax);
  right_motor_move(V * rightF, Vmax);
}