import processing.net.*;
import controlP5.*;

String COMMAND = "";




Client myClient;
String data;
String buggyIP = "192.168.4.1";
// STOP and GO command
// set speed ("V") or set distance ("D")
// set ref speed
// set distance
// set speed
// end line
// nothing
String[] command = {"X","S","V","0","0","0","E"};

float travelDist, objectDist;
float speedLeft, speedRight, avgSpeed;
float voltageLeft, voltageRight;
float refSpeed = 0.0;

color primaryColor    = #2c3e50;
color secondaryColor  = #3498db;
color dangerColor     = #e74c3c;
color successColor    = #2ecc71;
color warningColor    = #f1c40f;
color boxColor        = #34495e;
int windowWidth  = 1300;
int windowHeight = 900;

ControlP5 cp5;

Textlabel modeLabel;
Textarea logArea;

boolean isDistanceMode = false;
boolean isSpeedMode    = false;

void setup() {
  size(1300, 900);
  myClient = new Client(this, buggyIP, 5204);
  myClient.write("I am a nw clint");
  cp5 = new ControlP5(this);
  PFont font = createFont("Arial", 14);
  cp5.setFont(font);
  createDashboardElements();
  createNewElements();
}

void draw() {
  background(primaryColor);
  
  // US Sensor Distance section moved further to the right
  drawUSDistanceSection();
  
  // Desired Distance and Desired Speed sections remain (with increased vertical spacing)
  drawDesiredDistanceBox();
  drawDesiredSpeedBox();
  
  // System Log heading
  fill(#ecf0f1);
  textSize(16);
  text("System Log", 30, 430);
  
  drawHeader();
  updateSensorData();
  displayConnectionStatus();
}

void drawUSDistanceSection() {
  // Draw background box for US sensor distance, shifted right by 50 px
  fill(boxColor);
  rect(750, 100, 200, 40);
  fill(#ecf0f1);
  textSize(16);
  text("US Sensor Distance", 760, 90);
  
  // Draw the bar representing the sensor reading
  float maxDist = 100;
  float barWidth = map(objectDist, 0, maxDist, 0, 180);
  fill(successColor);
  rect(760, 110, barWidth, 20);
  
  // Display the distance text
  fill(#ecf0f1);
  textSize(20);
  text(nf(objectDist, 1, 2) + " cm", 760, 130);
}

void drawDesiredDistanceBox() {
  fill(boxColor);
  rect(700, 240, 300, 70);
  fill(#ecf0f1);
  textSize(16);
  text("Desired Distance", 710, 230);
}

void drawDesiredSpeedBox() {
  fill(boxColor);
  rect(700, 360, 300, 70);
  fill(#ecf0f1);
  textSize(16);
  text("Desired Speed", 710, 350);
}

void drawHeader() {
  fill(secondaryColor);
  noStroke();
  rect(0, 0, width, 60);
  fill(#ffffff);
  textSize(24);
  text("Autonomous Vehicle Control System", 20, 40);
  fill(dangerColor);
  ellipse(width - 60, 30, 20, 20);
  fill(#ecf0f1);
  textSize(14);
  text("Wi-Fi Connection", width - 150, 35);
}

void displayConnectionStatus() {
  // Optionally update connection status here (currently shown in header)
}

void createDashboardElements() {
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
    
    
    
  
  cp5.addTextlabel("refSpeedTitle")
    .setText("Set Reference Speed")
    .setPosition(100, 100)
    .setColorValue(#ecf0f1);
    
  cp5.addTextfield("refSpeedInput")
    .setPosition(100, 130)
    .setSize(150, 30)
    .setColorValue(#2c3e50)
    .setColorBackground(#ecf0f1)
    .setAutoClear(false);
    
  cp5.addButton("setRefSpeed")
    .setPosition(260, 130)
    .setSize(80, 30)
    .setLabel("SET")
    .setColorBackground(secondaryColor)
    .setColorActive(#2980b9);
    
    
    
    
  modeLabel = cp5.addTextlabel("mode")
    .setText("Current Mode: STOPPED")
    .setPosition(30, 400)
    .setColorValue(#ecf0f1)
    .setFont(createFont("Arial Bold", 16));
  
  logArea = cp5.addTextarea("logArea")
    .setPosition(30, 450)
    .setSize(400, 250)
    .setFont(createFont("Arial", 14))
    .setLineHeight(24)
    .setColor(color(0))
    .setColorBackground(color(220))
    .setText("System started...\n");
}



void createNewElements() {
  // Desired Distance UI Elements
  cp5.addToggle("DistanceMode")
     .setPosition(710, 255)
     .setSize(40, 40)
     .setValue(false)
     .setColorBackground(dangerColor)
     .setLabel("") // Remove label text
     .setFont(createFont("Arial", 16));
     
  cp5.addTextfield("desiredDistanceInput")
     .setPosition(760, 255)
     .setSize(130, 40)
     .setColorValue(#2c3e50)
     .setColorBackground(#ecf0f1)
     .setAutoClear(false)
     .setFont(createFont("Arial", 16));
     
  cp5.addButton("setDesiredDistance")
     .setPosition(900, 255)
     .setSize(90, 40)
     .setLabel("Set")
     .setColorBackground(secondaryColor)
     .setColorActive(#2980b9)
     .setFont(createFont("Arial", 16));
     
     
     
  // Desired Speed UI Elements
  cp5.addToggle("SpeedMode")
     .setPosition(710, 375)
     .setSize(40, 40)
     .setValue(false)
     .setColorBackground(dangerColor)
     .setLabel("") // Remove label text
     .setFont(createFont("Arial", 16));
     
  cp5.addTextfield("desiredSpeedInput")
     .setPosition(760, 375)
     .setSize(130, 40)
     .setColorValue(#2c3e50)
     .setColorBackground(#ecf0f1)
     .setAutoClear(false)
     .setFont(createFont("Arial", 16));
     
  cp5.addButton("setDesiredSpeed")
     .setPosition(900, 375)
     .setSize(90, 40)
     .setLabel("Set")
     .setColorBackground(secondaryColor)
     .setColorActive(#2980b9)
     .setFont(createFont("Arial", 16));
}



void controlEvent(ControlEvent e) {
  if (e.isFrom("DistanceMode")) {
    if (cp5.get(Toggle.class, "DistanceMode").getState()) {
      cp5.get(Toggle.class, "SpeedMode").setValue(false);
      cp5.get(Toggle.class, "DistanceMode").setColorBackground(successColor);
      cp5.get(Toggle.class, "SpeedMode").setColorBackground(dangerColor);
      cp5.get(Button.class, "setDesiredDistance").setColorBackground(successColor);
      cp5.get(Button.class, "setDesiredSpeed").setColorBackground(secondaryColor);
      isDistanceMode = true;
      isSpeedMode = false;
      command[2] = "V";
    } else {
      cp5.get(Toggle.class, "DistanceMode").setColorBackground(dangerColor);
      cp5.get(Button.class, "setDesiredDistance").setColorBackground(secondaryColor);
      isDistanceMode = false;
    }
  } else if (e.isFrom("SpeedMode")) {
    if (cp5.get(Toggle.class, "SpeedMode").getState()) {
      cp5.get(Toggle.class, "DistanceMode").setValue(false);
      cp5.get(Toggle.class, "SpeedMode").setColorBackground(successColor);
      cp5.get(Toggle.class, "DistanceMode").setColorBackground(dangerColor);
      cp5.get(Button.class, "setDesiredSpeed").setColorBackground(successColor);
      cp5.get(Button.class, "setDesiredDistance").setColorBackground(secondaryColor);
      isSpeedMode = true;
      isDistanceMode = false;
      command[2] = "D";
    } else {
      cp5.get(Toggle.class, "SpeedMode").setColorBackground(dangerColor);
      cp5.get(Button.class, "setDesiredSpeed").setColorBackground(secondaryColor);
      isSpeedMode = false;
    }
  }
}

void updateSensorData() {
  if (myClient.available() > 0) {
    data = myClient.readString();
    if (data != null) {
      String[] splitData = split(data, ',');
      if (splitData.length >= 7) {
        try {
          travelDist   = float(splitData[0]);
          objectDist   = float(splitData[1]);
          speedLeft    = float(splitData[2]);
          speedRight   = float(splitData[3]);
          voltageLeft  = float(splitData[4]);
          voltageRight = float(splitData[5]);
          String buggyMsg = splitData[6];
          logArea.append("Buggy Message: " + buggyMsg + "\n");
          logArea.scroll(1);
        } catch (NumberFormatException e) {
          println("Error reading sensor data: " + e.getMessage());
        }
      }
    }
  }
}

void STOP() {
  command[1] = "S";
  sendCommand();
  modeLabel.setText("Current Mode: EMERGENCY STOP").setColorValue(dangerColor);
}

void GO() {
  command[1] = "G";
  sendCommand();
  modeLabel.setText("Current Mode: MOVE FORWARD").setColorValue(successColor);
}

void setRefSpeed() {
  try {
    refSpeed = float(cp5.get(Textfield.class, "refSpeedInput").getText());
    command[3] = str(refSpeed);
    sendCommand();
    logArea.append("Reference Speed set to: " + refSpeed + " cm/s\n");
  } catch (Exception e) {
    println("Invalid speed value!");
  }
}

void setDesiredDistance() {
  try {
    float desiredDistance = float(cp5.get(Textfield.class, "desiredDistanceInput").getText());
    command[4] = str(desiredDistance);
    sendCommand();
    logArea.append("Desired Distance set to: " + desiredDistance + " cm\n");
  } catch (Exception e) {
    println("Invalid distance value!");
  }
}

void setDesiredSpeed() {
  try {
    float desiredSpeed = float(cp5.get(Textfield.class, "desiredSpeedInput").getText());
    command[5] = str(desiredSpeed);
    sendCommand();
    logArea.append("Desired Speed set to: " + desiredSpeed + " cm/s\n");
  } catch (Exception e) {
    println("Invalid speed value!");
  }
}

void sendCommand() {
  if (myClient.active()) {
    for (int i = 0 ; i < command.length ; i++){
      COMMAND += command[i];
    }
    myClient.write(COMMAND);
    println("Command sent: " + COMMAND);
    logArea.append("Command sent: " + COMMAND + "\n");
  }
}
