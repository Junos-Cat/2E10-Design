#pragma once
#include <Arduino.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

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
float x = 0;
float y = 0;
// Loaded
// const int speedForward = 50;
// const int speedOuter = 60;
// const int speedInner = 20;
// const int speedStop = 0;

// Unloaded (for calibration)
const int speedForward = 50;
const int speedOuter = 90;
const int speedInner = 0;
const int speedStop = 0;

const int vForward = 2.8; //min: 2.7 niceish: 2.8 max: 3
const int vOuter = 2.8;
const int vInner = 1;

// Unloaded
const int distanceForward = 15;
const int distanceOuter = 16;
const int distanceInner = 14;
const int distanceStop = 0;

const float leftF = 1.25;     // Correction factor for left motor
const float rightF = 1;       // Correction factor for right motor

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
float USSensorDuration, USSensorDistance, USSensorDistancePrevious = 0;     // Duration of ultrasonic pulse and calculated distance

// --- Commands to and from the laptop ---
String messageOut;
String dataIn;
String RPMString;
String DistanceString;

// Encoder/PID variables
int leftEncoderCount = 0;
int rightEncoderCount = 0;

int leftTheta = 0;
int rightTheta = 0;
int leftThetaPrevious = 0;
int rightThetaPrevious = 0;
int leftDeltaTheta;
int rightDeltaTheta;

float Vmax = 7.2;
float Vmin = 0; // if we are able to reverse, we can't accuratley measure RPM
float V = 0;
float VPrevious;

float E;
float EPrevious = 0;
float Inte;
float IntePrevious = 0;
float Diff;

float leftRPM;
float leftRPMPrevious = 0;
float currentLeftRPM;

float rightRPM;
float rightRPMPrevious = 0;
float currentRightRPM;

float RPMDesired = 35;
float maxRPM = 45;
float DistanceDesired = 15;

float pidDesiredTCounter = 0;
bool change = true;

const float damper = 0;

int UIMode = 1;

///////////////////////////https://www.youtube.com/watch?v=uXnDwojRb1g
// To check if the steps really are present due to the steps in encoder values,
// plot the delta degrees (multiply by a factor if you need to make it more visable) 
// and if the steps in degree size corresponds to the steps in measured RPM, then
// the hypothesis is correct
const float kpSpeed = 0.003; // perfect
const float kiSpeed = 0.0012;
const float kdSpeed = 0.0001;

float kpDistance;
const float kiDistance = 0;
const float kdDistance = 0;//-0.001;//3;

const float dpr = 166.6666666;
const float half = 0.5;

// Encoder sensor pins
const int LEFT_ENCODER = 2;
const int RIGHT_ENCODER = 3;
float travelDistance = 0, leftTravelDistance = 0;
float rightTravelDistance, distancePerTheta = 0.05667;//15cm/360deg
float averageTravelDistance =0;

// Huskylens variables
HUSKYLENS huskylens;
String huskyMode = "Forward";
bool huskyTurning = false;
const int speedLimit1 = 25;
int CenterXPos, CenterYPos, BlockWidth, BlockHeight, ID;