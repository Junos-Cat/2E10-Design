#pragma once
#include <Arduino.h>
#include "variables.h"

void serialPlotter(){
  Serial.print(",");
  Serial.print("leftRPM:");
  Serial.print(leftRPM);

  Serial.print(",");
  Serial.print("leftRPMDesired:");
  Serial.print(leftRPMDesired);

  Serial.print(",");
  Serial.print("LowerV:");
  Serial.print(-5);
  Serial.print(",");
  Serial.print("UpperV:");
  Serial.print(260);

  Serial.print(",");
  Serial.print("LowerRPM:");
  Serial.print(-25);
  Serial.print(",");
  Serial.print("UpperRPM:");
  Serial.print(270);

  Serial.println();
}