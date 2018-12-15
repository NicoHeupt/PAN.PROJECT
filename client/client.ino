/*
	Client

  IOT client for ESP8266 that can connect to a Wifi, get a value from
  a web address and regulates the brightness of an LED by that value.

	The circuit:
	* Adafruit Feather HUZZAH ESP8266
	* LED with series resistor

	Created 15. December 2018
	By the PAN-Gang

	https://github.com/NicoHeupt/PAN.PROJECT

*/

// Libraries
#include <ESP8266WiFi.h> // contains functions to use ESP8266's Wifi
#include <ESP8266HTTPClient.h> // contains functions to use HTTP protocol
 
// WiFi Credentials
const char* ssid = "Wanderhure";   // name of the network
const char* password = "3kreuzer"; // WiFi key

const int ledPin = 4; // the Pin that the LED is connected to

// Global Variables
int sensorReading; // value as recieved by the server
int ledBrightness; // value that is put out of the pin
int getValueDelay = 300;  // milliseconds between http requests

void setup () {
 
  pinMode(ledPin, OUTPUT);

  // Connect to the network
  Serial.begin(115200); // open a serial interface
  WiFi.begin(ssid, password); // connect to WiFi

  // try to connect every 300 milliseconds until success
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300); 
  }
 
}
 
void loop() {
 
  // check if WiFi is connected
  if (WiFi.status() == WL_CONNECTED) {
 
    HTTPClient http;  // declare object of class HTTPClient
 
    http.begin("http://192.168.8.163/brightness");  // server adress (change  to your server's address)
                                                    // to avoid sensor override by server change /brightness to /sensor 
    int httpCode = http.GET(); //Send the request
 
    if (httpCode > 0) { // if the Return Code isn't >0 the request failed

      String payload = http.getString();     // get the request response payload as String
      Serial.println("payload: " + payload);
      sensorReading = payload.toInt();       // convert payload string to an int
      ledBrightness = calcBrightness(sensorReading); // calculate brightness from payload
      Serial.println("ledBrightness: " + String(ledBrightness));

      analogWrite(ledPin, ledBrightness); // write ledBrightness to the pin
    }
    http.end(); // close connection
 
  }
  delay(getValueDelay); // wait for x milliseconds
 
}

// calculate what is written to the pin
int calcBrightness(int sensorReading) {
  // add logic to suit your needs
  return sensorReading;
}