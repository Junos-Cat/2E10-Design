#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(double voltageVal, double voltageValPrevious, double measuredVal, double desiredVal, double error, double voltageVal2, double voltageValPrevious2, double measuredVal2, double desiredVal2, double error2){
  Serial.print(",");
  Serial.print("rightV:");
  Serial.print(voltageVal*25);

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
  Serial.print("leftV:");
  Serial.print(voltageVal2*25);

  // Serial.print(",");
  // Serial.print("rightVPrevious:");
  // Serial.print(voltageValPrevious2*25);

  Serial.print(",");
  Serial.print("leftDistance:");
  Serial.print(measuredVal2);

  Serial.print(",");
  Serial.print("leftDistanceDesired:");
  Serial.print(desiredVal2);

  // Serial.print(",");
  // Serial.print("rightE:");
  // Serial.print(error2);


  Serial.print(",");
  Serial.print("min:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("max:");
  Serial.print(180);

  Serial.println();
}