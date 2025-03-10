#pragma once
#include <Arduino.h>
#include <PID_v1.h>
#include "variables.h"

void pidSetup(){
  leftPID.SetMode(AUTOMATIC);
  leftPID.SetTunings(kp, ki, kd);
  rightPID.SetMode(AUTOMATIC);
  rightPID.SetTunings(kp, ki, kd);
}

void pid(int leftRPMDesired, int rightRPMDesired){
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
  rightRPMDesired = 0;

  // Calculate the RPM of each wheel
  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  leftRPM = map(leftDeltaTheta/(dt)*dpr, 0, 200, 0, 255);
  rightRPM = map(rightDeltaTheta/(dt)*dpr, 0, 200, 0, 255);
  leftPID.Compute();
  rightPID.Compute();
  analogWrite(LEFT_MOTOR_EN, leftV);
  analogWrite(RIGHT_MOTOR_EN, rightV);
}