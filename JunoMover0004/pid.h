#pragma once
#include <Arduino.h>
#include <PID_v1.h>
#include "variables.h"

void pidSetup(){
  leftSpeedPID.SetMode(AUTOMATIC);
  leftSpeedPID.SetTunings(kp, ki, kd);
  rightSpeedPID.SetMode(AUTOMATIC);
  rightSpeedPID.SetTunings(kp, ki, kd);
  leftDistancePID.SetMode(AUTOMATIC);
  leftDistancePID.SetTunings(kp, ki, kd);
  rightDistancePID.SetMode(AUTOMATIC);
  rightDistancePID.SetTunings(kp, ki, kd);
}

void pidSpeedMode(){
  pidDesiredTCounter += dt;

  // Step function that changes the desired RPM of the left motor
  if (pidDesiredTCounter>10000){
    pidDesiredTCounter-=10000;
    if (change){
      change = false;
      leftRPMDesired = random(60, 200);
    }
    else{
      change = true;
      leftRPMDesired = 0;
    }
  }
  // We're currently tuning the left wheel so we don't need this
  rightRPMDesired = 0;

  leftSpeedPID.Compute();
  rightSpeedPID.Compute();
  analogWrite(LEFT_MOTOR_EN, leftV);
  analogWrite(RIGHT_MOTOR_EN, rightV);
}
void pidDistanceMode(){
  pidDesiredTCounter += dt;

  // Need to tune in situ
  
  leftDistancePID.Compute();
  rightDistancePID.Compute();
  analogWrite(LEFT_MOTOR_EN, leftV);
  analogWrite(RIGHT_MOTOR_EN, rightV);
}