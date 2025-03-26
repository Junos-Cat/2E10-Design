#include <WiFiS3.h>
#include "processCommand.h"
#include "detectUSSensorObject.h"
#include "pid.h"
#include "variables.h"
#include "encoderInterrupt.h"
#include "pidCalibrationPrinting.h"
#include "HUSKYLENS.h"
HUSKYLENS huskylens;
#include "Myhuskylens.h"



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

  myHuskySetup();

  delay(3000);  // Delay to allow the system to stabilize
  Serial.println("Starting loop");
  //delay(1000);
  // runBuggy = true;
  UIMode = 1;
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

  leftRPM = (currentLeftRPM + leftRPMPrevious)*half;
  rightRPM = (currentRightRPM + rightRPMPrevious)*half;

  // Check incoming HTTP request
  WiFiClient client = server.available();
  processCommand(client);

  // --- Ultrasonic Sensor Reading ---
  // If the robot is running, check the ultrasonic sensor every 500ms
  if (USTimeElapsed > 500) {
    USTimeElapsed = 0;
    detectUSSensorObject();
  }
  // detectUSSensorObject();

  if (MessageTimeElapsed > 500){
    MessageTimeElapsed = 0;
    // Send data to the laptop client
    messageOut = String("T") + "," + String(averageTravelDistance) + "," + String(USSensorDistance) + "," + String(leftRPM) + "," + String(rightRPM) + "," + String((leftRPM + rightRPM)/2) + "," ;
    client.println(messageOut);
  }
  
  huskyFunction();

  if (!runBuggy) {
    // left_motor_move(0, Vmax);
    // right_motor_move(0, Vmax);
    RPMDesired = 0;
    pid();
  } else {
    // Decide movement based on sensor input
    if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == HIGH) {
      pid();
      // Forward
      x=0;
      y=0;
      if (huskyTurning){ // If we have already taken the turn, then reset the huskyMode
        huskyTurning = false; // Say we're past the turn/have executed the QR code's command
        huskyMode == "Stop"; // Because we've completed the command and don't have any others, stop next time we see a junction
      }
    } else if (digitalRead(LEFT_IR) == LOW && digitalRead(RIGHT_IR) == HIGH) {
      // Turn Left
      x+=0.3;
      left_motor_move(vInner * leftF, Vmax);
      right_motor_move((vOuter + x) *rightF, Vmax);
    } else if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == LOW) {
      // Turn Right
      y+=0.3;
      left_motor_move((vOuter + y) * leftF, Vmax);
      right_motor_move(vInner * rightF, Vmax);
    } else { // Both IR sensors read white
      huskyTurning = true;
      if (huskyMode == "Stop"){ // Stop buggy
        left_motor_move(0, Vmax);
        right_motor_move(0, Vmax);
      }
      if (huskyMode == "Forward"){ // We're in the go forward mode
        pid();
        x=0;
        y=0;
      }
      else if (huskyMode == "Left"){ // We're in the turn left mode
        x+=0.3;
        left_motor_move(vInner * leftF, Vmax);
        right_motor_move((vOuter + x) *rightF, Vmax);
      }
      else if (huskyMode == "Right"){ // We're in the turn right mode
        left_motor_move((vOuter + y) * leftF, Vmax);
        right_motor_move(vInner * rightF, Vmax);
        y+=0.3;
      }
    }
  }
  if (UIMode == 1){
    serialPlotter(V, VPrevious, leftRPM, rightRPM, RPMDesired, E);
  }
  else if (UIMode == 2){
    serialPlotter(V, VPrevious, USSensorDistance, USSensorDistance, DistanceDesired, E);
  }

  // Update time for next loop iteration
  tPrevious = t;
  leftThetaPrevious = leftTheta;
  rightThetaPrevious = rightTheta;
  USSensorDistancePrevious = USSensorDistance;
  leftRPMPrevious = leftRPM;
  rightRPMPrevious = rightRPM;
}