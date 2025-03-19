#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){
  if (USSensorDistance < 50 && mode == 2){
    E = USSensorDistance - DistanceDesired;

    if (E > 0){
      kpDistance = 0.1;
    }
    else{
      kpDistance = 5;
    }
    if (E > 40){
      E = 40;
    }

    // Integration
    Inte = IntePrevious + (dt * (E + EPrevious) * half);

    // Differenciation
    Diff = (E - EPrevious) / dt;

    // Voltage
    V = VPrevious + (kpDistance * E + kiDistance * Inte + kdDistance * Diff);
    // if (V>4.5){
    //   V = 4.5;
    // }
  }
  else {
    // Error for speed
    E = RPMDesired - (leftRPM + rightRPM)/2;

    // Integration
    Inte = IntePrevious + (dt * (E + EPrevious) * half);

    // Differenciation
    Diff = (E - EPrevious) / dt;

    // Voltage
    V = VPrevious + (kpSpeed * E + kiSpeed * Inte + kdSpeed * Diff);
  }

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