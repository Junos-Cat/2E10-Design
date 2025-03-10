#pragma once
#include <Arduino.h>
#include "variables.h"
#include "motorControl.h"

void pid(int sideMotor, int speed){
  pidDesiredTCounter += dt;

  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;

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
  
  rightVDesired = 0;

  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  leftRPM = leftDeltaTheta/(dt)*dpr;
  if ((-5 < leftRPM || leftRPM < 5) && not(-0.25 < leftV || leftV < 0.25)){
    leftRPM = leftRPMPrevious;
  }
  rightRPM = rightDeltaTheta/(dt)*dpr;
  if ((-5 < rightRPM || rightRPM < 5) && not(-0.25 < rightV || rightV < 0.25)){
    rightRPM = rightRPMPrevious;
  }//dont need anymore
  
  // leftRPM = 60;
  // rightRPM = 80;
  // Calculation of error, integration value, 
  // and voltage to be supplied to a motor
  leftE = leftRPMDesired - leftRPM;
  leftInte = leftIntePrevious + (dt * (leftE + leftEPrevious) * half);
  // if (leftInte > 10){
  //   leftInte -= damper;
  // }
  // else if (leftInte < -10){
  //   leftInte += damper;
  // }
  // else {
  //   leftInte = 0;
  // }
  leftV = (kp * leftE + ki * leftInte + (kd * (leftE - leftEPrevious))*1000 / dt);

  rightE = rightRPMDesired - rightRPM;
  rightInte = rightIntePrevious + (dt * (rightE + rightEPrevious) * half);

  // if (rightInte > 10){
  //   rightInte -= damper;
  // }
  // else if (rightInte < -10){
  //   rightInte += damper;
  // }
  // else {
  //   rightInte = 0;
  // }

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