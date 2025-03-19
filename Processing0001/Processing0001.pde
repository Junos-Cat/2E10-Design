import processing.net.*;
import controlP5.*;

String COMMAND = "";
String refSpeedString;
String refSpeedString1;
String desiredDistanceString;
String desiredDistanceString1;
String desiredDistanceString2;
String desiredSpeedString;
String desiredSpeedString1;
String desiredSpeedString2;


float radius = 100; // Radius for the speedometer
float arcAngle = 240; // The arc of the speedometer (240 degrees)
float minSpeed = 0; // Minimum speed (0)
float maxSpeed = 200; // Maximum speed (*TO BE CHANGED)
float currentSpeed = 50; // Initial speed (50 cm/s)
float leftWheelSpeed = 30; // Random initial value
float rightWheelSpeed = 70; // Random initial value

// sensors
int Sensor_x_baseline = 150;
int Sensor_y_baseline = 150;
int Sensor_y_spacing = 80;

// inputs
int Input_x_baseline = 750;
int Input_y_baseline = 180;
int Input_y_spacing = 120;

// speedometer
float x = 750;
float y = 550;
float smallY = y + 170;
  
Client myClient;
String data;
String buggyIP = "192.168.4.1";
// STOP and GO command
// set speed ("V") or set distance ("D")
// set distance
// set speed
// end line
// nothing
String[] command = {"X","S","V","000","000","E"};

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
  updateSensorData();
  // Draw Sensors
  drawUSDistanceSection();
  drawDistanceTravelledSection();

  
  // Desired Distance and Desired Speed sections remain (with increased vertical Sensor_y_spacing)
  drawDesiredDistanceBox();
  drawDesiredSpeedBox();
  drawSpeedometer();
  
  // System Log heading
  fill(#ecf0f1);
  textSize(16);
  text("System Log", 30, 430);
  
  drawHeader();
  
  displayConnectionStatus();
}

//%%%%%%%%%%%%%         Speedomeeter          %%%%%%%%%%%%%%%%%%%

void drawSpeedometer() {
  // Draw the average speed speedometer

  // Call the function to draw the large speedometer
  drawLargeSpeedometer(x, y, radius, avgSpeed);

  // Draw the smaller speedometers below
  float smallRadius = 45; // Smaller radius for the two smaller speedometers


  // Left Speedometer
  drawSmallSpeedometer(x - 150, smallY, smallRadius, speedLeft);

  // Right Speedometer
  drawSmallSpeedometer(x + 150, smallY, smallRadius, speedRight);

  // Draw titles below each small speedometer
  textSize(12);
  textAlign(CENTER, CENTER);
  text("Left Wheel", x - 150, smallY + 55); // Title for Left Wheel Speedometer
  text("Right Wheel", x + 150, smallY + 55); // Title for Right Wheel Speedometer

  // Title for Average Speed
  textSize(14);
  text("Avg Speed", x, y + radius + 5);
}

//******************************************     sensor boxes       ****************************
void drawUSDistanceSection() {
  // Draw background box for US sensor distance, shifted right by 50 px
  fill(boxColor);
  rect(Sensor_x_baseline - 70, Sensor_y_baseline + 10, 200, 40);
  fill(#ecf0f1);
  textSize(16);
  text("US Sensor Distance", Sensor_x_baseline, Sensor_y_baseline - 10);
  
  // Draw the bar representing the sensor reading
  float maxDist = 100;
  float barWidth = map(objectDist, 0, maxDist, 0, 180);
  fill(successColor);
  //rect(760, 110, barWidth, 20);
  
  // Display the distance text
  fill(#ecf0f1);
  textSize(20);
  text(nf(objectDist, 1, 2) + " cm", Sensor_x_baseline, Sensor_y_baseline + 30);
}

void drawDistanceTravelledSection() {
  // Draw background box for US sensor distance, shifted right by 50 px
  fill(boxColor);
  rect(Sensor_x_baseline - 70, Sensor_y_baseline + Sensor_y_spacing + 10, 200, 40);
  fill(#ecf0f1);
  textSize(16);
  text("Distance Travelled", Sensor_x_baseline, Sensor_y_baseline - 10 + Sensor_y_spacing);
  
  //println(travelDist);
  
  // Draw the bar representing the sensor reading
  float maxDist = 100;
  float barWidth = map(travelDist, 0, maxDist, 0, 180);
  fill(successColor);
  //rect(760, 110, barWidth, 20);
  
  // Display the distance text
  fill(#ecf0f1);
  textSize(20);
  text(nf(travelDist, 1, 2) + " cm", Sensor_x_baseline, Sensor_y_baseline + 30 + Sensor_y_spacing);
}


void drawLargeSpeedometer(float x, float y, float radius, float currentSpeed) {
  // Draw the arc of the speedometer
  stroke(0);
  strokeWeight(2);
  noFill();
  pushMatrix();
  translate(x, y);
  rotate(PI); // Speedometer keeps printing upside down, needs to be rotated

  // Start 30 degrees below horizontal to the left
  arc(0, 0, radius * 2, radius * 2, -radians(30), -radians(30) + radians(arcAngle));

  // Draw baseline joining end of the arc
  float arcStartX = cos(-radians(30)) * radius;  // Left end of arc
  float arcStartY = sin(-radians(30)) * radius;
  float arcEndX = cos(-radians(30) + radians(arcAngle)) * radius;  // Right end of arc
  float arcEndY = sin(-radians(30) + radians(arcAngle)) * radius;

  line(arcStartX, arcStartY, arcEndX, arcEndY); // Horizontal base

  // Draw the arc bar representing current speed
  float angle = map(currentSpeed, minSpeed, maxSpeed, -radians(30), -radians(30) + radians(arcAngle));
  stroke(0, 0, 255); // Blue for now
  strokeWeight(10);
  noFill();
  arc(0, 0, radius * 2, radius * 2, -radians(30), angle);

  popMatrix();

  // Labels and values for main speedometer
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(16);
  text(currentSpeed + " cm/s", x, y); // Add speed to centre
}

void drawSmallSpeedometer(float x, float y, float radius, float currentSpeed) {
  // Draw the arc of the smaller speedometer
  stroke(0);
  strokeWeight(2);
  noFill();
  pushMatrix();
  translate(x, y);
  rotate(PI); // Same issue with smaller speedometers, need to be rotated

  arc(0, 0, radius * 2, radius * 2, -radians(30), -radians(30) + radians(arcAngle));

  float arcStartX = cos(-radians(30)) * radius;  // Left end of arc
  float arcStartY = sin(-radians(30)) * radius;
  float arcEndX = cos(-radians(30) + radians(arcAngle)) * radius;  // Right end of arc
  float arcEndY = sin(-radians(30) + radians(arcAngle)) * radius;

  line(arcStartX, arcStartY, arcEndX, arcEndY); // Horizontal base

  // Draw the arc bar for the current speed
  float angle = map(currentSpeed, minSpeed, maxSpeed, -radians(30), -radians(30) + radians(arcAngle));
  stroke(0, 0, 255);
  strokeWeight(5);
  noFill();
  arc(0, 0, radius * 2, radius * 2, -radians(30), angle);

  pushMatrix();
  rotate(PI);
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(14);
  text(currentSpeed + " cm/s", 0, 0);
  popMatrix();

  popMatrix();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,     draw input boxes        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

void drawDesiredDistanceBox() {
  fill(boxColor);
  rect(Input_x_baseline, Input_y_baseline + 10, 300, 70);
  fill(#ecf0f1);
  textSize(16);
  text("Desired Distance", Input_x_baseline + 60, Input_y_baseline);
}

void drawDesiredSpeedBox() {
  fill(boxColor);
  rect(Input_x_baseline, Input_y_baseline + Input_y_spacing + 10, 300, 70);
  fill(#ecf0f1);
  textSize(16);
  text("Desired Speed", Input_x_baseline + 60, Input_y_baseline + Input_y_spacing);
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

//_____________________________     Stop and go buttons     _________________

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
    
    
    
    
    //{{{{{{{{{{{{{{{{{{{{{{{{     Log tables      }}}}}}}}}}}}}}}}}}}}}}}}}}
 
    
    
    
    
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

//PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP    Distance   PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP

void createNewElements() {
  // Desired Distance UI Elements
  cp5.addToggle("DistanceMode")
     .setPosition(Input_x_baseline + 10, Input_y_baseline + 25)
     .setSize(40, 40)
     .setValue(false)
     .setColorBackground(dangerColor)
     .setLabel("") // Remove label text
     .setFont(createFont("Arial", 16));
     
  cp5.addTextfield("desiredDistanceInput")
     .setPosition(Input_x_baseline + 50 + 10, Input_y_baseline + 25)
     .setSize(130, 40)
     .setColorValue(#2c3e50)
     .setColorBackground(#ecf0f1)
     .setAutoClear(false)
     .setFont(createFont("Arial", 16))
     .setLabel(""); // Hide the label
     
  cp5.addButton("setDesiredDistance")
     .setPosition(Input_x_baseline + 190 + 10, Input_y_baseline + 25)
     .setSize(90, 40)
     .setLabel("Set")
     .setColorBackground(secondaryColor)
     .setColorActive(#2980b9)
     .setFont(createFont("Arial", 16));
     
//++++++++++++++++++++++++++++++++++++++++++    Speed    +++++++++++++++++++++++++++++++++
     
  // Desired Speed UI Elements
  cp5.addToggle("SpeedMode")
     .setPosition(Input_x_baseline + 10, Input_y_baseline + Input_y_spacing + 25)
     .setSize(40, 40)
     .setValue(false)
     .setColorBackground(dangerColor)
     .setLabel("") // Remove label text
     .setFont(createFont("Arial", 16));
     
  cp5.addTextfield("desiredSpeedInput")
     .setPosition(Input_x_baseline + 50 + 10, Input_y_baseline + Input_y_spacing + 25)
     .setSize(130, 40)
     .setColorValue(#2c3e50)
     .setColorBackground(#ecf0f1)
     .setAutoClear(false)
     .setFont(createFont("Arial", 16))
     .setLabel(""); // Hide the label
     
  cp5.addButton("setDesiredSpeed")
     .setPosition(Input_x_baseline + 190 + 10, Input_y_baseline + Input_y_spacing + 25)
     .setSize(90, 40)
     .setLabel("Set")
     .setColorBackground(secondaryColor)
     .setColorActive(#2980b9)
     .setFont(createFont("Arial", 16));
}

//??????????????????????????????????????????????????  Mode buttons  ????????????????????????????????????????????
 
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
      command[2] = "D";
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
      command[2] = "V";
    } else {
      cp5.get(Toggle.class, "SpeedMode").setColorBackground(dangerColor);
      cp5.get(Button.class, "setDesiredSpeed").setColorBackground(secondaryColor);
      isSpeedMode = false;
    }
  }
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  Data in  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
void updateSensorData() {
  if (myClient.available() > 0) {
    data = myClient.readString();
    //println(data);
    if (data != null) {
      String[] splitData = split(data, ',');
      if (splitData.length >= 6) {
        //println(splitData[0]);
        //print("sub");
        //print(data.substring(0,1));
        //println("end");
        try {
          //if (data.substring(0,1) == "T"){///////////////////////////////IDK IF THIS IS WHERE IM GOING WRONG
            println(splitData);
          travelDist   = float(splitData[1]);
          objectDist   = float(splitData[2]);
          speedLeft    = float(splitData[3]);
          speedRight   = float(splitData[4]);
          avgSpeed     = float(splitData[5]);
          //voltageRight = float(splitData[6]);
          //String buggyMsg = splitData[6];
          //logArea.append("Buggy Message: " + buggyMsg + "\n");
          //logArea.scroll(1);
          //}
        } catch (NumberFormatException e) {
          println("Error reading sensor data: " + e.getMessage());
        }
      }
    }
  }
}
//------------------------------------------------  Buttons  ----------------------------------------------------
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

void setDesiredDistance() {
  try {
    float desiredDistance = int(cp5.get(Textfield.class, "desiredDistanceInput").getText());
    desiredDistanceString = str(desiredDistance);
    if (desiredDistanceString.length() == 3){
      desiredDistanceString1 = "00" + desiredDistanceString;
    }
    else if (desiredDistanceString.length() == 4){
      desiredDistanceString1 = "0" + desiredDistanceString;
    }
    else{
      desiredDistanceString1 = desiredDistanceString;
    }
    desiredDistanceString2 = desiredDistanceString1.substring(0,3);
    command[3] = desiredDistanceString2;
    sendCommand();
    logArea.append("Desired Distance set to: " + desiredDistanceString2 + " cm\n");
  } catch (Exception e) {
    println("Invalid distance value!");
  }
}

void setDesiredSpeed() {
  try {
    float desiredSpeed = int(cp5.get(Textfield.class, "desiredSpeedInput").getText());
    println(desiredSpeed);
    desiredSpeedString = str(desiredSpeed);
    println(desiredSpeedString);
    
    if (desiredSpeedString.length() == 3){
      desiredSpeedString1 = "00";
      desiredSpeedString1 += desiredSpeedString;
    }
    else if (desiredSpeedString.length() == 4){
      desiredSpeedString1 = "0";
      desiredSpeedString1 += desiredSpeedString;
    }
    else {
      desiredSpeedString1 = desiredSpeedString;
    }
    
    desiredSpeedString2 = desiredSpeedString1.substring(0,3);
    println(desiredSpeedString2);
  
    command[4] = desiredSpeedString2;
    sendCommand();
    logArea.append("Desired Speed set to: " + desiredSpeedString2 + " cm/s\n");
  } catch (Exception e) {
    println("Invalid speed value!");
  }
}

void sendCommand() {
  if (myClient.active()) {
    COMMAND = "";
    for (int i = 0 ; i < command.length ; i++){
      COMMAND += command[i];
    }
    myClient.write(COMMAND);
    println("Command sent: " + COMMAND);
    logArea.append("Command sent: " + COMMAND + "\n");
  }
}
