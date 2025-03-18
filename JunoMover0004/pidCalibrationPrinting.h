#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double voltageVal, double voltageValPrevious, double measuredVal, double desiredVal, double error, double measuredVal2){
  Serial.print(",");
  Serial.print("V:");
  Serial.print(voltageVal*25);

  // Serial.print(",");
  // Serial.print("leftVPrevious:");
  // Serial.print(voltageValPrevious*25);

  Serial.print(",");
  Serial.print("rightRPM:");
  Serial.print(measuredVal);

  Serial.print(",");
  Serial.print("Desired:");
  Serial.print(desiredVal);

  // Serial.print(",");
  // Serial.print("leftE:");
  // Serial.print(error);

  Serial.print(",");
  Serial.print("rightRPM:");
  Serial.print(measuredVal2);

  // Serial.print(",");
  // Serial.print("rightE:");
  // Serial.print(error2);


  Serial.print(",");
  Serial.print("min:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("max:");
  Serial.print(150);

  Serial.println();
}