#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "Wanderhure";
const char* password = "3kreuzer";
const byte ledPin = 4;

int sensorReading;
int ledBrightness;

// calculate the value to put through the analog pin
int calcBrightness(int sensorReading) {
  // add logic to your
  return sensorReading;
}
 
void setup () {
 
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting..");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300); 
  }
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.8.163/sensor");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
      Serial.println("payload: " + payload);           //Print the response payload

      sensorReading = payload.toInt();

      ledBrightness = calcBrightness(sensorReading);
      Serial.println("ledBrightness: " + String(ledBrightness));                     //Print the response payload

      analogWrite(ledPin, ledBrightness);
    }
    http.end();   //Close connection
 
  }
  delay(300);    //Send a request every 300 milliseconds
 
}