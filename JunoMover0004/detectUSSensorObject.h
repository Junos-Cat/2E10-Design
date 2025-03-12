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
  usSensorDuration = pulseIn(US_ECHO, HIGH, 30000);
  if (usSensorDuration > 0) {
    // Convert duration to distance in cm
    usSensorDistance = usSensorDuration / 58.0;
    if (usSensorDistance <= 10) { // If an obstacle is too close (10 cm threshold)
      USStop = true;
    } else { // No obstacle
      USStop = false;
    }
  } else {
    USStop = false;
  }
}