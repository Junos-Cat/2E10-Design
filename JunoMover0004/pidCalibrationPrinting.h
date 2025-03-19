#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double voltageVal, double voltageValPrevious, double measuredVal, double measuredVal1, double desiredVal, double error){
  Serial.print(",");
  Serial.print("V:");
  Serial.print(voltageVal*25);

  // Serial.print(",");
  // Serial.print("leftVPrevious:");
  // Serial.print(voltageValPrevious*25);

  Serial.print(",");
  Serial.print("leftRPM:");
  Serial.print(measuredVal);

  Serial.print(",");
  Serial.print("rightRPM:");
  Serial.print(measuredVal1);

  Serial.print(",");
  Serial.print("Desired:");
  Serial.print(desiredVal);

  Serial.print(",");
  Serial.print("E:");
  Serial.print(error);

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