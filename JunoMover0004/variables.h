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
const int speed0 = 0;
const int speed1 = 140;       // Base speed for forward motion
const int speed2 = 140;       // Base speed for turning (adjustable)
const int speed3 = 0;         // Speed for stop
const float innerTurnFactor = 0.8;     // Correction factor for left motor
const float outerTurnFactor = 1.2;       // Correction factor for right motor
float x = 0;                  // Variable for incremental adjustments (e.g., turning)
float y = 0;
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
float USTimeElapsed = 0, MessageTimeElapsed = 0;        // Accumulates elapsed time for periodic tasks
float t, tPrevious=0, dt;
float duration, distance, distancePrevious = 0;     // Duration of ultrasonic pulse and calculated distance
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

float Vmax = 6;
float Vmin = 0; // if we are able to reverse, we can't accuratley measure RPM
double leftV = 0;
double rightV = 0;

double leftRPM;
double leftRPMDesired;
double rightRPM;
double rightRPMDesired;

float pidDesiredTCounter = 0;
bool change = true;

const float damper = 0;

///////////////////////////https://www.youtube.com/watch?v=uXnDwojRb1g
// To check if the steps really are present due to the steps in encoder values,
// plot the delta degrees (multiply by a factor if you need to make it more visable) 
// and if the steps in degree size corresponds to the steps in measured RPM, then
// the hypothesis is correct
double kp = 0.01;// we can't just set this to as inte is also dependant on e
double ki = 0;
double kd = 0;

PID leftPID(&leftRPM, &leftV, &leftRPMDesired, kp, ki, kd, 0);
PID rightPID(&rightRPM, &rightV, &rightRPMDesired, kp, ki, kd, 0);

const float dpr = 166.6666666;
const float half = 0.5;

// Encoder sensor pins
const int LEFT_ENCODER = 2;
const int RIGHT_ENCODER = 3;
float travelDistance = 0, leftTravelDistance = 0;
float rightTravelDistance, distancePerTheta = 0.05667;//15cm/360deg
float averageTravelDistance =0;