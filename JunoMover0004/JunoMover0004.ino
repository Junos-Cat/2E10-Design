#include <WiFiS3.h>
#include "processCommand.h"
#include "detectUSSensorObject.h"
#include "pid.h"

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
const int speed1 = 140;       // Base speed for forward motion
const int speed2 = 140;       // Base speed for turning (adjustable)
const int speed3 = 0;         // Speed for stop
const float leftF = 1.15;     // Correction factor for left motor
const float rightF = 1;       // Correction factor for right motor
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
int leftTheta = 0;
int rightTheta = 0;
int leftThetaPrevious = 0;
int rightThetaPrevious = 0;
int leftDeltaTheta;
int rightDeltaTheta;
float t, tPrevious=0, dt;
float duration, distance, distancePrevious = 0;     // Duration of ultrasonic pulse and calculated distance
String message;

// Encoder/pid variables
volatile int leftEncoderCount = 0;
volatile int rightEncoderCount = 0;
bool interupt = false;

// Encoder sensor pins
const int LEFT_ENCODER = 2;
const int RIGHT_ENCODER = 3;
float travelDistance = 0, leftTravelDistance = 0;
float rightTravelDistance, distancePerTheta = 0.05667;//15cm/360deg
float averageTravelDistance =0;
// Encoder interrupt functions
void left_encoder_interrupt(){
  // if (interupt){
    leftEncoderCount = leftEncoderCount + 1;
    //Serial.println("--------");
  //}
}

void right_encoder_interrupt(){
  if (interupt){
    rightEncoderCount ++;
    //Serial.println("+++++++++");
  }
}
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
  interupt = true;
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
  if (runBuggy && USTimeElapsed > 500) {
    detectUSSensorObject();
  }

  if (MessageTimeElapsed > 500){
    MessageTimeElapsed = 0;
    // Send data to the laptop client
    message = String(leftEncoderCount) + "," + String(distance) + ",";
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
      pid(LEFT_MOTOR_EN, speed1 * leftF);
      pid(RIGHT_MOTOR_EN, speed1 * rightF);
      x = 0;
      y = 0;
    } else if (digitalRead(LEFT_IR) == LOW && digitalRead(RIGHT_IR) == HIGH) {
      x += 0.1;
      pid(LEFT_MOTOR_EN, speed3 * leftF);
      pid(RIGHT_MOTOR_EN, (speed2 + x) * rightF);
    } else if (digitalRead(LEFT_IR) == HIGH && digitalRead(RIGHT_IR) == LOW) {
      y += 0.1;
      pid(LEFT_MOTOR_EN, (speed2 + y) * leftF);
      pid(RIGHT_MOTOR_EN, speed3 * rightF);
    } else {
      // If no sensor condition is met, stop the motors
      pid(LEFT_MOTOR_EN, 0);
      pid(RIGHT_MOTOR_EN, 0);
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



