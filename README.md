# PAN.PROJECT
<b>IOT- Projekt VIII<b><br />
<b>LICHTSENSOR<b>



Getting started with Arduino?
Wanna get confortable within a easy little Project?<br />
Degree of difficulty: easy<br />
<b>Then you´re absolutly right with PAN!<b>
  
Here you´ll see a little overview about the Project:<br />
As already thought the PAN Project is about a light sensor, in which you can control different brightness levels.<br />
The Client regulate those configurations. Moreover you have the opportunity to manage the Arduino with your Smartphone via WIFI <br />

<b>ANYAWAY, HAVE FUN!<b>



1. Please Install the Arduino Software IDE within the following Link: https://www.arduino.cc/en/Main/Software

2. Starting position: 
  - Stack the upper body (Feather Huzzah 8266 Wifi) on the Adalogger.
  - In the Software choose following pre-settings:
    - Select Adafruit Feather HUZZAH ESP8266 from the Tools->Board dropdown
    - Choose correct Port
  - Overview Pinout Adafruit-Feather-Huzzah: https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/pinouts
    
3. Cabling<br />
  - Please refer to PAN.Project -> Hardware for the Image: https://github.com/NicoHeupt/PAN.PROJECT/tree/master/hardware

4.Coding<br />
  - Please refer to PAN.Project -> Code: 
  
5. Connecting with your Smartphone<br />
  - Open your Browser and fill in your Ip- Address
  - Important: Client and Server have to be in the same network.
  

## Ressources
[Adalogger Dokumentation](https://learn.adafruit.com/adafruit-feather-m0-adalogger/using-the-sd-card#)
-> __chipSelect = 15;__



# Client
The client consists of an Adafruit Feather HUZZAH ESP8266 and an LED as actor. The LED brightness is regulated via [pulse width modulation](https://www.arduino.cc/en/pmwiki.php?n=Reference/AnalogWrite). The Huzzah gets a sensor reading from the server via Wifi and from that calculates a brightness value to put out.
## Hardware
Connect the LED with a resistor in front of it to pin 4 (labled "SDA" on the Huzzah). Notice that LEDs (Light Emitting Diodes), as all diodes, only allow current to pass in one direction. It won't work the LED's the wrong way around.
## Configuration
* Enter credentials for your WiFi
  ```c
  // WiFi Credentials
  const char* ssid = "myWifiSSID";    // name of the network
  const char* password = "myWifiKey"; // WiFi key
  ```
* If you use a different board than the Adafruit Feather HUZZAH ESP8266 make sure to have the correct pin number and set ledPin accordingly
  ```c
  const int ledPin = 4; // the Pin that the LED is connected to
  ```  
* Set adress to your brightness value. This will normally be `http://ServerIP/brightness`. If you need a value that can't be overridden by the server you can use `http://ServerIP/sensor`.   For more information refer to the [Server section](#Server) of this file
  ```c
  http.begin("http://192.168.0.23/brightness");  // server adress (change to your server's address)
  ```
* Set how often this client gets the brightness value. from the server
  ```c
  int getValueDelay = 300;  // milliseconds between http requests
  ```
* Write your calcBrightness! By default clients will put the recieved brightness value through to the pin. This behaviour can easily be changed to suit your needs. Check the following section to get an idea!
### calcBrightness examples
_TODO:_ write a few examples

# Server
## Configuration