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

  

  // Integration
  Inte = IntePrevious + (dt * (E + EPrevious) * half);

  // Differenciation
  Diff = (E - EPrevious) / dt;

  // Voltage
  if (mode == 1){
    V = VPrevious + (kpSpeed * E + kiSpeedAc * Inte + kdSpeedAc * Diff);
  }
  else if (mode == 2){
    V = VPrevious + (kpDistance * E + kiDistance * Inte + kdDistance * Diff);
    if (V>4.5){
      V = 4.5;
    }
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

  if (mode == 1){
  left_motor_move(V, Vmax);
  right_motor_move(V, Vmax);
  }
  else if (mode == 2){
    left_motor_move(V * leftFDistance, Vmax);
    right_motor_move(V * rightFDistance, Vmax);
  }
}