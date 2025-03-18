#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double voltageVal, double voltageValPrevious, double measuredVal, double desiredVal, double error, double measuredVal2){
  Serial.print(",");
  Serial.print("rightV:");
  Serial.print(voltageVal*2);

  // Serial.print(",");
  // Serial.print("leftVPrevious:");
  // Serial.print(voltageValPrevious*25);

  Serial.print(",");
  Serial.print("rightDistance:");
  Serial.print(measuredVal);

  Serial.print(",");
  Serial.print("rightDistanceDesired:");
  Serial.print(desiredVal);

  // Serial.print(",");
  // Serial.print("leftE:");
  // Serial.print(error);

  Serial.print(",");
  Serial.print("leftDistance:");
  Serial.print(measuredVal2);

  // Serial.print(",");
  // Serial.print("rightE:");
  // Serial.print(error2);


  Serial.print(",");
  Serial.print("min:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("max:");
  Serial.print(50);

  Serial.println();
}