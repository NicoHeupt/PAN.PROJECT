#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Wanderhure";
const char* password = "3kreuzer";
const char* serverIP = "192.168.8.163";

void setup () {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password); // assuming SSID and Passphrase already in flash
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }

}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin(serverIP);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
    }

    http.end();   //Close connection
  }
 
  delay(1000);    //Send a request every 3 seconds
 
}