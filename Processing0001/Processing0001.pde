import processing.net.*;

Client myClient;
String data, travMessage;
float objectDistPrev, objectDistPrevPrev, objectDist, travelDist;
String buggyIP = "192.168.4.1";
String command = "STOP";
String message = "na", messagePrev = "NA";

// Button variables
int buttonWidth = 100;
int buttonHeight = 50;
color stopColor = color(255, 0, 0);
color goColor = color(0, 255, 0);
color bgColor = color(200); // Default background color

void setup() {
  myClient = new Client(this, buggyIP, 5204);
  myClient.write("I am a new client");
  size(400, 200);
}

void draw() {
  background(bgColor);
  
  // Draw Stop button
  fill(stopColor);
  rect(50, 75, buttonWidth, buttonHeight, 10);
  fill(255);
  textSize(20);
  textAlign(CENTER, CENTER);
  text("STOP", 50 + buttonWidth / 2, 75 + buttonHeight / 2);
  
  // Draw Go button
  fill(goColor);
  rect(250, 75, buttonWidth, buttonHeight, 10);
  fill(255);
  text("GO", 250 + buttonWidth / 2, 75 + buttonHeight / 2);
  
  if (myClient.available() > 0) {
    data = myClient.readString();
    println(data);
    
    if (data != null) {
      String[] splitData = split(data, ',');  // Split the string at the comma
      
      // Convert the split strings into floats
      travelDist = float(splitData[0]);
      objectDist = float(splitData[1]);
      
      // Display the received distance and message on the screen
      fill(0);
      textSize(16);
      textAlign(LEFT, TOP);
      travMessage = "Distance: " + travelDist;

      
      if (objectDist < 10) {
        message = "Message: Object detected";
      } else {
        if (objectDistPrev < 10) {
          message = "Message: Object removed, proceeding";
        } else {
          message = "Message: No object detected";
        }
      }
      objectDistPrev = objectDist;  // Update the previous object distance#
      messagePrev = message;
    }
    text(travMessage, 10, 10);
    text(message, 10, 40);
    
  }
  
  delay(500);
}

void mousePressed() {
  if (mouseX > 50 && mouseX < 50 + buttonWidth && mouseY > 75 && mouseY < 75 + buttonHeight) {
    command = "S";
  } else if (mouseX > 250 && mouseX < 250 + buttonWidth && mouseY > 75 && mouseY < 75 + buttonHeight) {
    command = "G";
  }
  sendCommand(command);
}

void sendCommand(String command) {
  myClient.write(command);
  println(command);
}
