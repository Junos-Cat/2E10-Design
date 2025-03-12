import processing.net.*;
import controlP5.*;

Client myClient;
String data, travelMessage;
float objectDistPrev, objectDistPrevPrev, objectDist, travelDist;
String buggyIP = "192.168.0.193";
String command = "STOP";
String message = "na", messagePrev = "NA";

// ControlP5 objects
ControlP5 cp5;
Textlabel avgSpeedLabel, wheelSpeedsLabel, distanceLabel, modeLabel, messageLabel, voltageLabel, wifiLabel, refSpeedLabel;
float avgSpeed, speedLeft, speedRight, voltageLeft, voltageRight, refSpeed;

// UI Constants
color primaryColor = #2c3e50;
color secondaryColor = #3498db;
color dangerColor = #e74c3c;
color successColor = #2ecc71;
color warningColor = #f1c40f;
int windowWidth = 1000;
int windowHeight = 800;

void setup() {
  // Set the window size
  size(1000, 800);
  
  // Create a client to connect to the buggy
  myClient = new Client(this, buggyIP, 5204);
  myClient.write("I am a new client");
  
  cp5 = new ControlP5(this);
  
  // Set font and style settings
  PFont font = createFont("Arial", 14);
  cp5.setFont(font);
  
  createDashboardElements();
}

void draw() {
  background(primaryColor);
  drawHeader();
  updateSensorData();
  displayConnectionStatus();
}

void createDashboardElements() {
  // Info Panel
  int panelY = 80;
  int panelSpacing = 40;
  
  avgSpeedLabel = cp5.addTextlabel("avgSpeed")
                    .setText("Average Speed: 0.0 cm/s")
                    .setPosition(30, panelY)
                    .setColorValue(#ecf0f1);

  wheelSpeedsLabel = cp5.addTextlabel("wheelSpeeds")
                       .setText("Wheel Speeds: Left=0.0 cm/s | Right=0.0 cm/s")
                       .setPosition(30, panelY + panelSpacing)
                       .setColorValue(#ecf0f1);

  distanceLabel = cp5.addTextlabel("distance")
                    .setText("Distance Travelled: 0.0 cm")
                    .setPosition(30, panelY + panelSpacing*2)
                    .setColorValue(#ecf0f1);

  voltageLabel = cp5.addTextlabel("voltage")
                   .setText("Motor Voltages: Left=0.0V | Right=0.0V")
                   .setPosition(30, panelY + panelSpacing*3)
                   .setColorValue(#ecf0f1);

  messageLabel = cp5.addTextlabel("message")
                   .setText("Object Detection: No object detected")
                   .setPosition(30, panelY + panelSpacing*4)
                   .setColorValue(#ecf0f1);

  // Control Panel
  createControlButtons();
  createSpeedInput();
  
  // Mode Indicator
  modeLabel = cp5.addTextlabel("mode")
                .setText("Current Mode: STOPPED")
                .setPosition(30, 400)
                .setColorValue(#ecf0f1)
                .setFont(createFont("Arial Bold", 16));
}

void createControlButtons() {
  cp5.addButton("STOP")
     .setPosition(500, 300)
     .setSize(120, 60)
     .setColorBackground(dangerColor)
     .setColorActive(#c0392b)
     .setLabel("EMERGENCY STOP")
     .setFont(createFont("Arial Bold", 12));
     
  cp5.addButton("GO")
     .setPosition(500, 200)
     .setSize(120, 60)
     .setColorBackground(successColor)
     .setColorActive(#27ae60)
     .setLabel("MOVE")
     .setFont(createFont("Arial Bold", 12));
}

void createSpeedInput() {
  cp5.addTextlabel("refSpeedTitle")
     .setText("Set Reference Speed")
     .setPosition(500, 100)
     .setColorValue(#ecf0f1);
     
  cp5.addTextfield("refSpeedInput")
     .setPosition(500, 130)
     .setSize(150, 30)
     .setColorValue(#2c3e50)
     .setColorBackground(#ecf0f1)
     .setAutoClear(false);
     
  cp5.addButton("setRefSpeed")
     .setPosition(660, 130)
     .setSize(80, 30)
     .setLabel("SET")
     .setColorBackground(secondaryColor)
     .setColorActive(#2980b9);
}

void drawHeader() {
  fill(secondaryColor);
  noStroke();
  rect(0, 0, width, 60);
  fill(#ffffff);
  textSize(24);
  text("Autonomous Vehicle Control System", 30, 40);
}

void displayConnectionStatus() {
  fill(myClient.active() ? successColor : dangerColor);
  ellipse(750, 40, 20, 20);
  textSize(12);
  fill(#ecf0f1);
  text("Wi-Fi Connection", 660, 45);
}

void updateSensorData() {
  if (myClient.available() > 0) {
    data = myClient.readString();
    
    if (data != null) {
      String[] splitData = split(data, ',');
      
      if (splitData.length >= 6) {
        try {
          travelDist = float(splitData[0]);
          objectDist = float(splitData[1]);
          speedLeft = float(splitData[2]);
          speedRight = float(splitData[3]);
          voltageLeft = float(splitData[4]);
          voltageRight = float(splitData[5]);
          
          updateDashboardMetrics();
          updateObjectDetection();
        } catch (NumberFormatException e) {
          println("Error reading sensor data!");
        }
      }
    }
  }
}

void updateDashboardMetrics() {
  avgSpeed = (speedLeft + speedRight) / 2;
  avgSpeedLabel.setText("Average Speed: " + nf(avgSpeed, 0, 1) + " cm/s");
  wheelSpeedsLabel.setText("Wheel Speeds: Left=" + nf(speedLeft, 0, 1) + " cm/s | Right=" + nf(speedRight, 0, 1) + " cm/s");
  distanceLabel.setText("Distance Travelled: " + nf(travelDist, 0, 1) + " cm");
  voltageLabel.setText("Motor Voltages: Left=" + nf(voltageLeft, 0, 1) + "V | Right=" + nf(voltageRight, 0, 1) + "V");
}

void updateObjectDetection() {
  String statusMsg;
  if (objectDist < 10) {
    statusMsg = "WARNING: " + nf(objectDist, 0, 1) + " cm away!";
    messageLabel.setColorValue(warningColor);
  } else {
    statusMsg = "Safe distance: " + nf(objectDist, 0, 1) + " cm";
    messageLabel.setColorValue(successColor);
  }
  messageLabel.setText("Object Detection: " + statusMsg);
}

void STOP() {
  command = "S";
  sendCommand(command);
  modeLabel.setText("Current Mode: EMERGENCY STOP").setColorValue(dangerColor);
}

void GO() {
  command = "G";
  sendCommand(command);
  modeLabel.setText("Current Mode: MOVE FORWARD").setColorValue(successColor);
}

void setRefSpeed() {
  try {
    refSpeed = float(cp5.get(Textfield.class, "refSpeedInput").getText());
    myClient.write("R" + refSpeed);
    cp5.get(Textlabel.class, "refSpeedTitle").setText("Reference Speed: " + nf(refSpeed, 0, 1) + " cm/s");
  } catch (Exception e) {
    println("Invalid speed value!");
  }
}

void sendCommand(String command) {
  if (myClient.active()) {
    myClient.write(command);
    println("Sent command: " + command);
  }
}
