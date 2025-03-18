#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double voltageVal, double measuredVal, double desiredVal){
  Serial.print(",");
  Serial.print("leftRPM:");
  Serial.print(measuredVal);

  Serial.print(",");
  Serial.print("leftRPMDesired:");
  Serial.print(desiredVal);

  Serial.print(",");
  Serial.print("leftV:");
  Serial.print(voltageVal*25);

  Serial.print(",");
  Serial.print("leftVPrevious:");
  Serial.print(leftVPrevious*25);

  Serial.print(",");
  Serial.print("leftE:");
  Serial.print(leftE);

  Serial.print(",");
  Serial.print("min:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("max:");
  Serial.print(150);

  Serial.println();
}