#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double measuredVal, double desiredVal){
  Serial.print(",");
  Serial.print("leftRPM:");
  Serial.print(measuredVal);

  Serial.print(",");
  Serial.print("leftRPMDesired:");
  Serial.print(desiredVal);

  Serial.print(",");
  Serial.print("max:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("min:");
  Serial.print(500);

  Serial.println();
}