#pragma once
#include <WiFiS3.h>
#include <PID_v1.h>

#include "processCommand.h"
#include "detectUSSensorObject.h"
#include "pid.h"
#include "variables.h"
#include "encoderInterrupt.h"

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
  t = millis();
  dt = t - tPrevious;
  USTimeElapsed += dt;
  MessageTimeElapsed += dt;

  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;
  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  leftTravelDistance += leftDeltaTheta * distancePerTheta;
  rightTravelDistance += rightDeltaTheta * distancePerTheta;
  averageTravelDistance = (leftTravelDistance + rightTravelDistance)*0.5;

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
    message = String(leftEncoderCount) + "," + String(distance) + "," ;
    client.println(message);
  }
  
  // --- Motor Control ---
  // Stop the motors if the robot is not running or if an obstacle is detected
  if (!runBuggy || USStop) {
    pid(LEFT_MOTOR_EN, speed3);
    pid(RIGHT_MOTOR_EN, speed3);
    x = 0;
    y = 0;
  } else {
    // Decide movement based on sensor input
    if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == HIGH) {
      // Move forward
      if (mode = 1){// Speed mode
        leftRPMDesired = speed1
        pidSpeedMode(speed1, speed1);
      }
      else{// Distance mode
        pidDistanceMode(disiredDistance);
      }
      x = 0;
      y = 0;
    } else if (digitalRead(LEFT_IR) == LOW && digitalRead(RIGHT_IR) == HIGH) {
      // Turn right
      pidSpeedMode(speed3 * outerTurnFactor, speed2 * innerTurnFactor);
    } else if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == LOW) {
      // Turn left
      pidSpeedMode(speed2 * innerTurnFactor, speed3 * outerTurnFactor);
    } else {
      // If no sensor condition is met, stop the motors
      pidSpeedMode(speed0, speed0);
    }
  }
  
  // Update time for next loop iteration
  tPrevious = t;
  leftThetaPrevious = leftTheta;
  rightThetaPrevious = rightTheta;
  distancePrevious = distance;
  // if (DELAY > 0) {
  //   delay(20);
  //   analogWrite(LEFT_MOTOR_EN, 0);
  //   analogWrite(RIGHT_MOTOR_EN, 0);
  //   delay(DELAY);
  // }
}



