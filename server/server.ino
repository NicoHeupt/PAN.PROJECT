#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Wanderhure";
const char* password = "3kreuzer";
const int sensorPin = 0;

int sensorReading;
int overBrightness;
bool doOverride = 0;

ESP8266WebServer server(80);
 
String getCheckboxState(bool isChecked){
  if(isChecked){
    return "checked";
  } else {
    return "";
  }
}

bool checkboxArgToBool(String arg){
  if(arg == "on"){
    return 1;
  } else {
    return 0;
  }
}

int getBrightness(){
  if(doOverride){
    return overBrightness;
  } else {
    return sensorReading;
  }
}
 
void handle_root() {
  sensorReading = analogRead(sensorPin);
  overBrightness = server.arg("overVal").toInt();
  doOverride = checkboxArgToBool(server.arg("over"));

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

  server.send(200, "text/html", htmlString);
  delay(3000);
}

void handle_sensor() {
  sensorReading = analogRead(sensorPin);
  server.send(200, "text/plain", String(sensorReading));
  delay(100);
}

void handle_brightness() {
  sensorReading = analogRead(sensorPin);
  server.send(200, "text/plain", String(getBrightness()));
  delay(100);
}

void setup(void)
{
  pinMode(sensorPin, INPUT); // set analog pin as input 
  Serial.begin(115200); // open serial connection

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", handle_root);
  server.on("/sensor", handle_sensor);
  server.on("/brightness", handle_brightness);

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
}