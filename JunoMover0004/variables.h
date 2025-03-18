#pragma once
#include <Arduino.h>
#include <PID_v1.h>

// --- WiFi Settings ---
const char* ssid = "Cheeky";             // Your WiFi network name
const char* password = "CheekyBoy";        // Your WiFi password
WiFiServer server(5204);                    // Create an HTTP server on port 80

// --- Global State Variables ---
bool runBuggy = false;                    // Flag to determine if the robot should run

// --- Sensor and Motor Pin Definitions ---
const int LEFT_IR = A2;          // Left IR sensor pin
const int RIGHT_IR = A1;          // Right IR sensor pin
const int US_TRIG = 13;       // Ultrasonic sensor trigger pin
const int US_ECHO = 12;       // Ultrasonic sensor echo pin

// --- Speed and Control Parameters ---
const float leftFactorSpeed = 1;
const float rightFactorSpeed = 1;

const int speed0 = 0;
int speedDesired0 = 0;
int speedDesired1 = 250;       // Outer wheel speed (forward)
int speedDesired2 = 90;      // Inner wheel speed
const int speed1 = 90;
const int speed2 = 170;       // Base speed for turning (adjustable)


int distanceDesired = 20;
const float innerTurnFactorSpeed = 0.8;
const float outerTurnFactorSpeed = 1.2;
const float innerTurnFactorDistance = 1.2;
const float outerTurnFactorDistance = 0.8;

// Message stuff
float averageSpeed;
float leftSpeed;
float rightSpeed;
int mode = 1;

int DELAY = 0;                // Delay used in movement control
bool USStop = false;          // Flag to stop when an obstacle is detected by the ultrasonic sensor

// --- Motor Control Pin Definitions ---
const int LEFT_MOTOR_EN = 5;            // Left motor enable pin
const int RIGHT_MOTOR_EN = 6;           // Right motor enable pin
const int LEFT_MOTOR_DIR_1 = 8;         // Left motor direction pin 1
const int LEFT_MOTOR_DIR_2 = 7;         // Left motor direction pin 2
const int RIGHT_MOTOR_DIR_2 = 10;       // Right motor direction pin 1
const int RIGHT_MOTOR_DIR_1 = 11;       // Right motor direction pin 2

// --- Timing and Ultrasonic Sensor Variables ---
float USTimeElapsed = 0;
float MessageTimeElapsed = 0;        
float t, tPrevious=0, dt;
float usSensorDuration;
double usSensorDistance;
double usSensorDistancePrevious = 0;
String message;

// Encoder/PID variables
int leftEncoderCount = 0;
int rightEncoderCount = 0;

int leftTheta = 0;
int rightTheta = 0;
int leftThetaPrevious = 0;
int rightThetaPrevious = 0;
int leftDeltaTheta;
int rightDeltaTheta;

// PID
float Vmax = 6;
float Vmin = 0; // if we are able to reverse, we can't accuratley measure RPM
double leftV = 0;
double rightV = 0;

double leftRPM;
double leftRPMDesired;
double leftDistanceDesired;
double rightRPM;
double rightRPMDesired;
double rightDistanceDesired;

// PID step function
float pidDesiredTCounter = 0;
bool change = true;
int chance = 0;

///////////////////////////https://www.youtube.com/watch?v=uXnDwojRb1g
// To check if the steps really are present due to the steps in encoder values,
// plot the delta degrees (multiply by a factor if you need to make it more visable) 
// and if the steps in degree size corresponds to the steps in measured RPM, then
// the hypothesis is correct

// PID calibration coefficients
double kpLeftSpeed = 10;
double kiLeftSpeed = 0;
double kdLeftSpeed = 0;

double kpRightSpeed = 0;
double kiRightSpeed = 0;
double kdRightSpeed = 0;

double kpLeftDistance = 0;
double kiLeftDistance = 0;
double kdLeftDistance = 0;

double kpRightDistance = 0;
double kiRightDistance = 0;
double kdRightDistance = 0;

PID leftSpeedPID(&leftRPM, &leftV, &leftRPMDesired, kpLeftSpeed, kiLeftSpeed, kdLeftSpeed, 0);
PID rightSpeedPID(&rightRPM, &rightV, &rightRPMDesired, kpRightSpeed, kiRightSpeed, kdRightSpeed, 0);
PID leftDistancePID(&usSensorDistance, &leftV, &leftDistanceDesired, kpLeftDistance, kiLeftDistance, kdLeftDistance, 0);
PID rightDistancePID(&usSensorDistance, &rightV, &rightDistanceDesired, kpRightDistance, kiRightDistance, kdRightDistance, 0);

const float dpr = 166.6666666;
const float half = 0.5;

// Encoder sensor pins
const int LEFT_ENCODER = 2;
const int RIGHT_ENCODER = 3;
float travelDistance = 0, leftTravelDistance = 0;
float rightTravelDistance, distancePerTheta = 0.05667;//15cm/360deg
float averageTravelDistance =0;