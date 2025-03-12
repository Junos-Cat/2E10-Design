#pragma once
#include <WiFiS3.h>
#include <PID_v1.h>

#include "processCommand.h"
#include "detectUSSensorObject.h"
#include "pid.h"
#include "variables.h"
#include "encoderInterrupt.h"
#include "pidCalibrationPrinting.h"

void pid();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.print("Connecting to WiFi...");

  // Connect to the WiFi network
  WiFi.beginAP(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  //delay(2000);
  Serial.println("\nWiFi Connected!");
  Serial.print("Arduino IP: ");
  Serial.println(WiFi.localIP());

  // Start the HTTP server
  server.begin();

  // --- Set Pin Modes ---
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  
  // Motor enable and direction pins
  pinMode(LEFT_MOTOR_EN, OUTPUT);
  pinMode(LEFT_MOTOR_DIR_1, OUTPUT);
  pinMode(LEFT_MOTOR_DIR_2, OUTPUT);
  digitalWrite(LEFT_MOTOR_DIR_1, HIGH);
  digitalWrite(LEFT_MOTOR_DIR_2, LOW);
  
  pinMode(RIGHT_MOTOR_EN, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR_2, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR_1, OUTPUT);
  digitalWrite(RIGHT_MOTOR_DIR_2, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR_1, LOW);
  
  // Ultrasonic sensor pins
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);
  digitalWrite(US_TRIG, LOW);
  
  // Encoders
  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);

  // Set up interupting function
  attachInterrupt( digitalPinToInterrupt(LEFT_ENCODER), left_encoder_interrupt, CHANGE);
  attachInterrupt( digitalPinToInterrupt(RIGHT_ENCODER), right_encoder_interrupt, CHANGE);

  // PID set up
  pidSetup();

  delay(3000);  // Delay to allow the system to stabilize
  Serial.println("Starting loop");
}

void loop() {
  // this needs to be changed to accept a command from the processing to update the mode
  mode = 1;
  t = millis();
  dt = t - tPrevious;
  USTimeElapsed += dt;
  MessageTimeElapsed += dt;

  // if (dt > 100){

  // Convert to degrees
  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;

  // Calculate the change in degrees of each wheel
  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  // Calculate the RPM of each wheel
  leftRPM = map(leftDeltaTheta/(dt)*dpr, 0, 200, 0, 255);
  rightRPM = map(rightDeltaTheta/(dt)*dpr, 0, 200, 0, 255);

  // Calculate the speed of each wheel
  leftSpeed = leftRPM * distancePerTheta * 360;
  rightSpeed = rightRPM * distancePerTheta * 360;
  averageSpeed = (leftSpeed + rightSpeed) * 0.5;

  // Calculate the total distance travelled
  leftTravelDistance += leftDeltaTheta * distancePerTheta;
  rightTravelDistance += rightDeltaTheta * distancePerTheta;
  averageTravelDistance = (leftTravelDistance + rightTravelDistance) * 0.5;

  // Check incoming HTTP request
  WiFiClient client = server.available();
  processCommand(client);

  // --- Ultrasonic Sensor Reading ---
  // If the robot is running, check the ultrasonic sensor every 500ms
  if (USTimeElapsed > 500) {
    detectUSSensorObject();
  }

  if (MessageTimeElapsed > 500){
    MessageTimeElapsed = 0;
    // Send data to the laptop client
    message = String(averageSpeed) + "," + String(leftSpeed) + "," + String(rightSpeed) + "," + String(averageTravelDistance) + "," + String(usSensorDistance) + "," ;
    client.println(message);
  }
  
  // --- Motor Control ---
  // Stop the motors if the robot is not running or if an obstacle is detected
  if (!runBuggy || USStop) {
    analogWrite(LEFT_MOTOR_EN, speed0);
    analogWrite(RIGHT_MOTOR_EN, speed0);
    leftRPMDesired = 0;
    rightRPMDesired = 0;
    leftDistanceDesired = 0;
    rightDistanceDesired = 0;
  } else {
    // Decide movement based on sensor input
    if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == HIGH) {
      // Move forward
      if (mode = 1){// Speed mode
        leftRPMDesired = speedDesired;
        rightRPMDesired = speedDesired;
        pidSpeedMode();
      }
      else{// Distance mode
        leftDistanceDesired = distanceDesired;
        rightDistanceDesired = distanceDesired;
        pidDistanceMode();
      }
    } else if (digitalRead(LEFT_IR) == LOW && digitalRead(RIGHT_IR) == HIGH) {
      // Turn right
      if (mode = 1){// Speed mode
        leftRPMDesired = speedDesired * innerTurnFactorSpeed;
        rightRPMDesired = speedDesired * outerTurnFactorSpeed;
        pidSpeedMode();
      }
      else{// Distance mode
        leftDistanceDesired = distanceDesired * innerTurnFactorDistance;
        rightDistanceDesired = distanceDesired * outerTurnFactorDistance;
        pidDistanceMode();
      }
    } else if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == LOW) {
      // Turn left
      if (mode = 1){// Speed mode
        leftRPMDesired = speedDesired * innerTurnFactorSpeed;
        rightRPMDesired = speedDesired * outerTurnFactorSpeed;
        pidSpeedMode();
      }
      else{// Distance mode
        leftDistanceDesired = distanceDesired * outerTurnFactorDistance;
        rightDistanceDesired = distanceDesired * innerTurnFactorDistance;
        pidDistanceMode();
      }
    } else {
      // If no sensor condition is met, stop the motors
      analogWrite(LEFT_MOTOR_EN, speed0);
      analogWrite(RIGHT_MOTOR_EN, speed0);
    }
    serialPlotter();
  }
  
  
  // Update time for next loop iteration
  tPrevious = t;
  leftThetaPrevious = leftTheta;
  rightThetaPrevious = rightTheta;
  usSensorDistancePrevious = usSensorDistance;
  // if (DELAY > 0) {
  //   delay(20);
  //   analogWrite(LEFT_MOTOR_EN, 0);
  //   analogWrite(RIGHT_MOTOR_EN, 0);
  //   delay(DELAY);
  // }
}



