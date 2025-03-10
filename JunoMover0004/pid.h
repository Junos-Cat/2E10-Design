#pragma once
#include <Arduino.h>
#include "variables.h"

void pid(int sideMotor, int speed){
  pidDesired TCounter += dt;

  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;

  if (otherTCounter>10000){
    otherTCounter-=10000;
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

  //leftRPMDesired = leftRPMMax * (sin(2 * pi * 0.005 * t / 1000.0)) * sign(sin(2 * pi * 0.05 * t / 1000.0));
  //rightRPMDesired = rightRPMMax * (sin(2 * pi * 0.005 * t / 1000.0)) * sign(sin(2 * pi * 0.05 * t / 1000.0));
  // if (leftTheta < leftThetaPrevious){
  //   leftDeltaTheta = 360 + leftTheta - leftThetaPrevious;
  // }
  // else {
  //   leftDeltaTheta = leftTheta - leftThetaPrevious;
  // }

  // if (rightTheta < rightThetaPrevious){
  //   rightDeltaTheta = 360 + rightTheta - rightThetaPrevious;
  // }
  // else {
  //   rightDeltaTheta = rightTheta - rightThetaPrevious;
  // }
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
  if (leftInte > 10){
    leftInte -= damper;
  }
  else if (leftInte < -10){
    leftInte += damper;
  }
  else {
    leftInte = 0;
  }
  leftV = (leftVPrevious + kp * leftE + ki * leftInte + (kd * (leftE - leftEPrevious))*1000 / dt);

  rightE = rightRPMDesired - rightRPM;
  rightInte = rightIntePrevious + (dt * (rightE + rightEPrevious) * half);
  if (rightInte > 10){
    rightInte -= damper;
  }
  else if (rightInte < -10){
    rightInte += damper;
  }
  else {
    rightInte = 0;
  }
  rightV = (rightVPrevious + kp * rightE + ki * rightInte + (kd * (rightE - rightEPrevious))*1000 / dt);

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