#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){
  // pidDesiredTCounter += dt;
  // // Step function that changes the desired RPM of the left motor
  // if (pidDesiredTCounter>20000){
  //   pidDesiredTCounter-=20000;
  //   if (change){
  //     change = false;
  //     leftRPMDesired = 150;
  //   }
  //   else{
  //     change = true;
  //     leftRPMDesired = 0;
  //   }
  // }
  // // We're currently tuning the left wheel so we don't need this
  // rightVDesired = 0;

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
    V = VPrevious + (kpSpeed * E + kiSpeedAc * Inte + kdSpeedAc * Diff);
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