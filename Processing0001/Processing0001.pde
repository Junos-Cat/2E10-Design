import processing.net.*;
import controlP5.*;

Client myClient;
String data;
float objectDist, travelDist;
String buggyIP = "192.168.0.193";
String command = "STOP";

// Sensor variables for display
float avgSpeed, speedLeft, speedRight, voltageLeft, voltageRight, refSpeed;

// UI Constants
color primaryColor = #2c3e50;
color secondaryColor = #3498db;
color dangerColor = #e74c3c;
color successColor = #2ecc71;
color warningColor = #f1c40f;
int windowWidth = 1000;
int windowHeight = 800;

// ControlP5 objects
ControlP5 cp5;
Textlabel avgSpeedLabel, wheelSpeedsLabel, distanceLabel, modeLabel, messageLabel, voltageLabel;
  
// Additional graphical components:
Chart speedChart;      // Line chart for speed data
Chart voltageChart;    // Bar chart for motor voltages
Knob refSpeedGauge;    // Gauge for reference speed
Toggle wifiToggle;     // Wi-Fi connection status indicator

void setup() {
  size(1000, 800);
  
  // Create a client to connect to the buggy
  myClient = new Client(this, buggyIP, 5204);
  myClient.write("I am a new client");
  
  cp5 = new ControlP5(this);
  PFont font = createFont("Arial", 14);
  cp5.setFont(font);
  
  // Create basic dashboard elements (text labels, buttons, text field)
  createDashboardElements();
  
  // Create additional charts and gauges
  createChartsAndGauges();
}

void draw() {
  background(primaryColor);
  drawHeader();
  updateSensorData();
  displayConnectionStatus();
  
  // Update charts with the latest values:
  speedChart.push("avgSpeed", avgSpeed);
  speedChart.push("leftSpeed", speedLeft);
  speedChart.push("rightSpeed", speedRight);
  
  voltageChart.setData("leftVolt", new float[]{voltageLeft});
  voltageChart.setData("rightVolt", new float[]{voltageRight});
  
  refSpeedGauge.setValue(refSpeed);
}

///////////////////////
// Dashboard Creation
///////////////////////
void createDashboardElements() {
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
  
  createControlButtons();
  createSpeedInput();
  
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

/////////////////////////////
// Additional Charts & Gauges
/////////////////////////////
void createChartsAndGauges() {
  // Create a line chart for speed values
  speedChart = cp5.addChart("speedChart")
                  .setPosition(30, 500)
                  .setSize(400, 150)
                  .setRange(0, 100)    // Adjust as needed for speed range
                  .setView(Chart.LINE)
                  .setColorCaptionLabel(color(0))
                  .setStrokeWeight(2);
  // Add datasets for average, left, and right speeds
  speedChart.addDataSet("avgSpeed");
  speedChart.addDataSet("leftSpeed");
  speedChart.addDataSet("rightSpeed");
  speedChart.setColors("avgSpeed", color(255, 0, 0));    // Red
  speedChart.setColors("leftSpeed", color(0, 255, 0));   // Green
  speedChart.setColors("rightSpeed", color(0, 0, 255));  // Blue
  // Initialize each dataset with an array (history length, e.g., 60 data points)
  speedChart.setData("avgSpeed", new float[60]);
  speedChart.setData("leftSpeed", new float[60]);
  speedChart.setData("rightSpeed", new float[60]);
  
  // Create a bar chart for motor voltages
  voltageChart = cp5.addChart("voltageChart")
                    .setPosition(450, 500)
                    .setSize(300, 150)
                    .setRange(0, 12)  // Assuming 0-12V range
                    .setView(Chart.BAR)
                    .setColorCaptionLabel(color(0));
  // Add two datasets for left and right motor voltages
  voltageChart.addDataSet("leftVolt");
  voltageChart.addDataSet("rightVolt");
  voltageChart.setColors("leftVolt", color(0, 102, 255));   // Blue
  voltageChart.setColors("rightVolt", color(255, 150, 0));  // Orange
  voltageChart.setData("leftVolt", new float[]{0});
  voltageChart.setData("rightVolt", new float[]{0});
  
  // Create a gauge for reference speed using a Knob
  refSpeedGauge = cp5.addKnob("refSpeedGauge")
                     .setPosition(800, 500)
                     .setRadius(50)
                     .setRange(0, 100)  // Adjust as needed
                     .setValue(0)
                     .setLock(true);  // Display only
  refSpeedGauge.getCaptionLabel().setPaddingY(10);
  
  // Create a Wi-Fi connection status toggle (non-interactive)
  wifiToggle = cp5.addToggle("wifiToggle")
                  .setPosition(800, 600)
                  .setSize(50, 20)
                  .setValue(false)
                  .setMode(ControlP5.SWITCH)
                  .setLabel("WiFi");
  wifiToggle.getCaptionLabel().align(ControlP5.CENTER, ControlP5.BOTTOM_OUTSIDE);
  wifiToggle.setLock(true);
}

/////////////////////////
// Header & Status Display
/////////////////////////
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
  wifiToggle.setValue(myClient.active());
}

/////////////////////////
// Sensor Data Update
/////////////////////////
void updateSensorData() {
  if (myClient.available() > 0) {
    data = myClient.readString();
    
    if (data != null) {
      String[] splitData = split(data, ',');
      
       if (splitData.length >= 6) {
        try {
          speedAve = float(splitData[0]);
          speedLeft = float(splitData[1]);
          speedRıght = float(splitData[2]);
          dıstanceTravelled = float(splitData[3]);
          objectDıstance = float(splitData[4]);
          messageBuggy = float(splitData[5]);
          
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

//////////////////////
// Command Functions
//////////////////////
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
