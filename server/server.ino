#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid     = "Wanderhure";
const char* password = "3kreuzer";
const int sensorPin = 0;
byte sensorReading;

ESP8266WebServer server(80);
 
String webString; // String to display
 
void handle_root() {
  webString = "Whaddup, go to ./sensor to get some fresh data, homie"; //String(analogRead(0));
  server.send(200, "text/plain", webString);
  delay(3000);
}

void handle_sensor() {
    server.send(200, "text/plain", String(analogRead(0)));
    delay(100);
}

void setup(void)
{
  pinMode(0, INPUT); // set analog pin as input 
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

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
}