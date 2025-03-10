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

  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  leftRPM = leftDeltaTheta/(dt)*dpr;

  rightRPM = rightDeltaTheta/(dt)*dpr;

  // Calculation of error, integration value, 
  // and voltage to be supplied to a motor
  leftE = leftRPMDesired - leftRPM;
  leftInte = leftIntePrevious + (dt * (leftE + leftEPrevious) * half);

  leftV = (kp * leftE + ki * leftInte + (kd * (leftE - leftEPrevious))*1000 / dt);

  rightE = rightRPMDesired - rightRPM;
  rightInte = rightIntePrevious + (dt * (rightE + rightEPrevious) * half);

  rightV = (kp * rightE + ki * rightInte + (kd * (rightE - rightEPrevious))*1000 / dt);

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