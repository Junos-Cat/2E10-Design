#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(){
  pidDesiredTCounter += dt;

  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;

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

  // Error
  if (mode == 1){
    leftE = leftRPMDesired - leftRPM;
    rightE = rightRPMDesired - rightRPM;
  }
  else if (mode == 2){
    leftE = USSensorDistance - leftDistanceDesired;
    rightE = USSensorDistance - rightDistanceDesired;
  }

  // Integration
  leftInte = leftIntePrevious + (dt * (leftE + leftEPrevious) * half);
  rightInte = rightIntePrevious + (dt * (rightE + rightEPrevious) * half);

  // Differenciation
  leftDiff = (leftE - leftEPrevious) / dt;
  rightDiff = (rightE - rightEPrevious) / dt;

  // Voltage
  if (mode == 1){
    leftV = leftVPrevious + (kpLeftSpeed * leftE + kiLeftSpeed * leftInte + kdLeftSpeed * leftDiff);
    rightV = rightVPrevious + (kpRightSpeed * rightE + kiRightSpeed * rightInte + kdRightSpeed * rightDiff);
  }
  else if (mode == 2){
    leftV = leftVPrevious + (kpLeftDistance * leftE + kiLeftDistance * leftInte + kdLeftDistance * leftDiff);
    rightV = rightVPrevious + (kpRightDistance * rightE + kiRightDistance * rightInte + kdRightDistance * rightDiff);
  }

  // Prevention of antiwinder
  if (leftV > Vmax) {
    leftV = Vmax;
    leftInte = leftIntePrevious;
  }
  if (leftV < Vmin) {
    leftV = Vmin;
    leftInte = leftIntePrevious;
  }
  if (rightV > Vmax) {
    rightV = Vmax;
    rightInte = rightIntePrevious;
  }
  if (rightV < Vmin) {
    rightV = Vmin;
    rightInte = rightIntePrevious;
  }

  // Update voltages
  leftVPrevious = leftV;
  rightVPrevious = rightV;

  left_motor_move(leftV, Vmax);
  right_motor_move(rightV, Vmax);
}