/*
	Server

  IOT server for ESP8266 that can
   - connect to a Wifi
   - read an analogue sensor (photoresistor)
   - run a webserver
   - offer several values via http
   - run a website to change said values

	The circuit:
	* Adafruit Feather HUZZAH ESP8266
	* photo resistor with series resistor

	Created 15. December 2018
	By the PAN-Gang

	https://github.com/NicoHeupt/PAN.PROJECT

*/

// Libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials
const char* ssid = "YourSSID";
const char* password = "YourPw";

const int sensorPin = 0; // the pin that the photoresistor is connected to

// Global Variables
int sensorReading;   // value read from the sensor
int overBrightness;  // value to override sensorReading
bool doOverride = 0; // indicates if overBrightness should used instead of sensorReading

ESP8266WebServer server(80); // declare object of class ESP8266WebServer

void setup(void)
{
  pinMode(sensorPin, INPUT); // set analog pin as input 

  Serial.begin(115200); // open serial connection

  // connect to WiFi network
  WiFi.begin(ssid, password); // connect to Wifi
  
  // try to connect every 500 milliseconds until success
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // print IP address
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Handeling Requests
  // the server will run the according function when a client requests a URL
  server.on("/", handle_root);
  server.on("/sensor", handle_sensor);
  server.on("/brightness", handle_brightness);

  // start server
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
}


//// Request Handlers ////

void handle_root() {
  sensorReading = analogRead(sensorPin);
  overBrightness = server.arg("overVal").toInt();
  doOverride = checkboxArgToBool(server.arg("over"));

  // HTML Code for the web interface
  String htmlString = 
    "<!DOCTYPE html><html><title>PAN-Project</title><body><form><table style='border-spacing: 10px'>"
      "<tr>"
        "<td></td><td>SensorReading:<br></td><td>" + String(sensorReading) + "</td>"
      "</tr><tr>"
        "<td><input type='checkbox' name='over' " + getCheckboxState(doOverride) + "></td>"
        "<td>BrightnessOverride: </td><td>" + String(overBrightness) + "</td>"
      "</tr><tr>"
        "<td></td>"
        "<td><input type='range' name='overVal' max='1024' value='" + String(overBrightness) + "'></td>"
        "<td><input type='submit' value='Submit'></td>"
      "</tr>"
    "</table></form></body></html>";

  server.send(200, "text/html", htmlString); // send the web interface
  delay(3000);
}

void handle_sensor() {
  sensorReading = analogRead(sensorPin); // get fresh value from sensor
  server.send(200, "text/plain", String(sensorReading)); // send sensor reading
  delay(100);
}

void handle_brightness() {
  sensorReading = analogRead(sensorPin);
  server.send(200, "text/plain", String(getBrightness())); // send brightness (sensor reading or override)
  delay(100);
}


//// Helper functions ////

// get "checked" if true (for checkbox in html form)
String getCheckboxState(bool isChecked){
  if(isChecked){
    return "checked";
  } else {
    return ""; // return empty string
  }
}

// return true if "on"
bool checkboxArgToBool(String arg){
  if(arg == "on"){
    return 1;
  } else {
    return 0;
  }
}

// return brightness (either sensor value or override, depending on doOverride)
int getBrightness(){
  if(doOverride){
    return overBrightness; // return override value
  } else {
    return sensorReading;
  }
}