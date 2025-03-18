#include <WiFiS3.h>
#include "processCommand.h"
#include "detectUSSensorObject.h"
#include "pid.h"
#include "variables.h"
#include "encoderInterrupt.h"
#include "pidCalibrationPrinting.h"

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

  delay(3000);  // Delay to allow the system to stabilize
  Serial.println("Starting loop");
  //delay(1000);
  //runBuggy = true;
}

void loop() {
  t = millis();
  dt = t - tPrevious;
  USTimeElapsed += dt;
  MessageTimeElapsed += dt;

  // Angle calculation
  leftTheta = leftEncoderCount * 45;
  rightTheta = rightEncoderCount * 45;
  leftDeltaTheta = leftTheta - leftThetaPrevious;
  rightDeltaTheta = rightTheta - rightThetaPrevious;

  // Distance calculation
  leftTravelDistance += leftDeltaTheta * distancePerTheta;
  rightTravelDistance += rightDeltaTheta * distancePerTheta;
  averageTravelDistance = (leftTravelDistance + rightTravelDistance)*0.5;

  // Speed calculation
  currentLeftRPM = leftDeltaTheta/(dt)*dpr;
  currentRightRPM = rightDeltaTheta/(dt)*dpr;
  
  // // This is to increase the responsiveness of the motors
  // if (leftRPMPrevious > RPMDesired * half){// Running average
  //   leftRPM = (currentLeftRPM + leftRPMPrevious)*half;
  // }
  // else{// No average
  //   leftRPM = currentLeftRPM;
  // }
  // if (rightRPMPrevious > RPMDesired * half){// Running average
  //   rightRPM = (currentRightRPM + rightRPMPrevious)*half;
  // }
  // else{// No average
  //   rightRPM = currentRightRPM;
  // }

  leftRPM = (currentLeftRPM + leftRPMPrevious)*half;
  rightRPM = (currentRightRPM + rightRPMPrevious)*half;

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
    messageOut = 'T' + "," + String(averageTravelDistance) + "," + String(USSensorDistance) + "," + String(leftRPM) + "," + String(rightRPM) + "," + String((leftRPM + rightRPM)/2) + "," ;
    //client.println(messageOut);
  }
  
  // --- Motor Control ---
  // Stop the motors if the robot is not running or if an obstacle is detected
  if (!runBuggy || USStop) {
    RPMDesired = speedStop;
    pid();
  } else {
    // Decide movement based on sensor input
    if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == HIGH) {
      // Forward
      // if (mode == 1){
      //   RPMDesired = speedForward;
      // }
      // else if (mode == 2){
      //   DistanceDesired = distanceForward;
      // }
      // pid();
      left_motor_move(vForward, Vmax);
      right_motor_move(vForward, Vmax);
      x=0;
      y=0;

    } else if (digitalRead(LEFT_IR) == LOW && digitalRead(RIGHT_IR) == HIGH) {
      // Turn Left
      x+=0.5;
      left_motor_move(vInner, Vmax);
      right_motor_move(vOuter + x, Vmax);
    } else if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == LOW) {
      // Turn Right
      y+=0.5;
      left_motor_move(vOuter + y, Vmax);
      right_motor_move(vInner, Vmax);
    } else {
      // If no sensor condition is met, stop the motors
      RPMDesired = speedStop;
      pid();
    }
  
  serialPlotter(V, VPrevious, leftRPM, rightRPM, DistanceDesired, E);

  }
  // Calibrating wheel speed
  // serialPlotter(leftV, leftVPrevious, leftRPM, leftRPMDesired, leftE);
  // Speed
  // Distance
  //serialPlotter(rightV, rightVPrevious, USSensorDistance, rightDistanceDesired, rightE, leftV, leftVPrevious, USSensorDistance, leftDistanceDesired, leftE);
  
  // Update time for next loop iteration
  tPrevious = t;
  leftThetaPrevious = leftTheta;
  rightThetaPrevious = rightTheta;
  USSensorDistancePrevious = USSensorDistance;
  leftRPMPrevious = leftRPM;
  rightRPMPrevious = rightRPM;
}