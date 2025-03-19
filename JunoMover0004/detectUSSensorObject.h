#pragma once
#include <Arduino.h>
#include "variables.h"

void detectUSSensorObject(){
  USTimeElapsed = 0;
  // Trigger the ultrasonic sensor
  //digitalWrite(US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);
  // Measure the duration of the echo pulse
  USSensorDuration = pulseIn(US_ECHO, HIGH, 30000);
  if (USSensorDuration > 0) {
    // Convert duration to distance in cm
    USSensorDistance = USSensorDuration / 58.0;
    // if (USSensorDistance <= 0) { // If an obstacle is too close (10 cm threshold)
    //   USStop = true;
    // } else { // No obstacle
    //   USStop = false;
    // }
  } else {
    // USStop = false;
  }
}