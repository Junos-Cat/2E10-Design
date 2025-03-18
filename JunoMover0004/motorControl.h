#pragma once
#include <Arduino.h>
#include "variables.h"

void left_motor_move(float leftV, float Vmax){
  int PWMValue = int(255 * (abs(leftV) / Vmax));
  if (PWMValue > 255){
    PWMValue = 255;
  }
  if (leftV > 0){
    digitalWrite(LEFT_MOTOR_DIR_1, HIGH);
    digitalWrite(LEFT_MOTOR_DIR_2, LOW);
  }
  else if (leftV < 0){
    digitalWrite(LEFT_MOTOR_DIR_1, LOW);
    digitalWrite(LEFT_MOTOR_DIR_2, HIGH);
  }
  else{
    digitalWrite(LEFT_MOTOR_DIR_1, LOW);
    digitalWrite(LEFT_MOTOR_DIR_2, LOW);
  }
  analogWrite(LEFT_MOTOR_EN, PWMValue);
}

void right_motor_move(float rightV, float Vmax){
  int PWMValue = int(255 * (abs(rightV) / Vmax));
  if (PWMValue > 255){
    PWMValue = 255;
  }
  if (rightV > 0){
    digitalWrite(RIGHT_MOTOR_DIR_1, LOW);
    digitalWrite(RIGHT_MOTOR_DIR_2, HIGH);
  }
  else if (rightV < 0){
    digitalWrite(RIGHT_MOTOR_DIR_1, HIGH);
    digitalWrite(RIGHT_MOTOR_DIR_2, LOW);
  }
  else{
    digitalWrite(RIGHT_MOTOR_DIR_1, LOW);
    digitalWrite(RIGHT_MOTOR_DIR_2, LOW);
  }
  analogWrite(RIGHT_MOTOR_EN, PWMValue);
}