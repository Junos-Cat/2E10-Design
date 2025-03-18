#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(int sideMotor, int speed){
  pidDesiredTCounter += dt;

  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;

  // Step function that changes the desired RPM of the left motor
  if (pidDesiredTCounter>10000){
    pidDesiredTCounter-=10000;
    if (change){
      change = false;
      leftRPMDesired = 170;
    }
    else{
      change = true;
      leftRPMDesired = 0;
    }
  }
  // We're currently tuning the left wheel so we don't need this
  rightVDesired = 0;

  // Error
  if (mode == 1){
    leftE = leftRPMDesired - leftRPM;
    rightE = rightRPMDesired - rightRPM;
  }
  else if (mode == 2){
    leftE = leftDistanceDesired - USSensorDistance;
    rightE = rightDistanceDesired - USSensorDistance;
  }

  // Integration
  leftInte = leftIntePrevious + (dt * (leftE + leftEPrevious) * half);
  rightInte = rightIntePrevious + (dt * (rightE + rightEPrevious) * half);

  // Differenciation
  leftDiff = (leftE - leftEPrevious) / dt;
  rightDiff = (rightE - rightEPrevious) / dt;

  // Voltage
  leftV = leftVPrevious + (kp * leftE + ki * leftInte + kd * leftDiff);
  rightV = rightVPrevious + (kp * rightE + ki * rightInte + kd * rightDiff);

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

}