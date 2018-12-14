#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
const char* ssid = "Wanderhure";
const char* password = "3kreuzer";
const int ledPin = 4;

int ledBrightness;

// calculate the value to put out of the analog pin
// change this function to your needs
byte calcBrightness(byte sensorReading) {
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
      String payload = http.getString();   //Get the request response payload
      //ledBrightness = payload.ToInt();
      Serial.println(payload);                     //Print the response payload
      //analogWrite(ledPin, ledBrightness);
    }
    http.end();   //Close connection
 
  }
  delay(1000);    //Send a request every second
 
}