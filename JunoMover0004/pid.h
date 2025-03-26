#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){
  // Error for speed
  E = RPMDesired - (leftRPM + rightRPM)/2;

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