#pragma once
#include <Arduino.h>
#include <PID_v1.h>
#include "variables.h"

void pidSetup(){
  leftSpeedPID.SetMode(AUTOMATIC);
  leftSpeedPID.SetTunings(kpLeftSpeed, kiLeftSpeed, kdLeftSpeed);
  rightSpeedPID.SetMode(AUTOMATIC);
  rightSpeedPID.SetTunings(kpRightSpeed, kiRightSpeed, kdRightSpeed);
  leftDistancePID.SetMode(AUTOMATIC);
  leftDistancePID.SetTunings(kpLeftDistance, kiLeftDistance, kdLeftDistance);
  rightDistancePID.SetMode(AUTOMATIC);
  rightDistancePID.SetTunings(kpRightDistance, kiRightDistance, kdRightDistance);
}

void pidSpeedMode(){
  pidDesiredTCounter += dt;

  // Step function that changes the desired RPM of the left motor
  if (pidDesiredTCounter>10000){
    if (change){
      pidDesiredTCounter-=10000;
      change = false;
      if (chance == 1){
        chance = 2;
        leftRPMDesired = speedDesired1;
      }
      else {
        chance = 1;
        leftRPMDesired = speedDesired2;
      }
    }
    else{
      pidDesiredTCounter-=5000;
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