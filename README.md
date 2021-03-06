# PAN.PROJECT (IOT Project VIII)

![](misc/concept-drawing.jpg)

PAN.PROJECT is a small IOT demonstration to show how communication between small physical devices via Wifi can be done on the Arduino platform.

The goals are as follows:
* have a device that acts as an server and sends some data over a wireless network, that is based on sensor readings. The sensor will be photoresistor ("light sensor") in our example. Also have webinterface to read out the sensor and send a override value by choice.
* have one or more devices that act as clients. They will get the sensor value from the server and adjust the brightness of an LED according to that.

# Prerequisites
* at least two [Adafruit Feather HUZZAH ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview), a sensor (we use a photoresistor), an actor (we use an LED) and some resistors and wire.
* Download and install the [Arduino Software IDE](https://www.arduino.cc/en/Main/Software)
  * follow instructions on [this page](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide) to install driver and board packages
  * this repo also contains dotfiles and workspace files for [Visual Studio Code](https://code.visualstudio.com/). If you choose to work with VS Code you will still need everything above and also [this extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino).
* Basic understanding of electronic circuits and computer networks are helpful
* More useful info on the [Adafruit Feather HUZZAH ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview)

# Client
The client consists of an Adafruit Feather HUZZAH ESP8266 and an LED as actor. The LED brightness is regulated via [pulse width modulation](https://www.arduino.cc/en/pmwiki.php?n=Reference/AnalogWrite). The Huzzah gets a sensor reading from the server via Wifi and from that calculates a brightness value to put out.

## Hardware
Connect the LED with a resistor in front of it to pin 4 (labled "SDA" on the Huzzah). Notice that LEDs (Light Emitting Diodes), as all diodes, only allow current to pass in one direction. It won't work the LED's the wrong way around.
![](client/client_sketch.png)

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
* Set adress to your brightness value. This will normally be `http://ServerIP/brightness`. If you need a value that can't be overridden by the server you can use `http://ServerIP/sensor`. For more information refer to the [Server section](#Server) of this file
  ```c
  http.begin("http://192.168.0.23/brightness");  // server adress (change to your server's address)
  ```
* Set how often this client gets the brightness value. from the server
  ```c
  int getValueDelay = 300;  // milliseconds between http requests
  ```
* Write your calcBrightness! By default clients will put the recieved brightness value through to the pin. This behaviour can easily be changed to suit your needs. Check the following section to get an idea!

## Examples for calcBrightness functions

### Default: _the brighter the sensor, the brighter the led_
  ```c
  int calcBrightness(int sensorReading) {
    return sensorReading; // just pass the value through
  }
  ```

### "Crossing the Streams": _the darker the sensor, the brighter the led_
  ```c
  // inverting sensorReading and brightness
  int calcBrightness(int sensorReading) {
    return 1024-sensorReading;
  }
  ```

### "tripple threshold": _fixed led brightness for ranges_
  ```c
  // three states: on, dimmed, off
  int calcBrightness(int sensorReading) {
    if(0 <= sensorReading && sensorReading <= 256) { // if sensor between 0 - 255
      return 1000;                                   // brightnes is 1000
    }
    if(256 <= sensorReading && sensorReading <= 768) {  // if sensor between 0 - 255
      return 500;                                       // brightnes is 500
    }
    else {      // for higher values (or something out of bounds)
      return 0; // turn led off
    }
  }
  ```

### "recursive rampage": _brave nerds go crazy_
  ```c
  // ramp up brightness from sensor reading to max value
  int calcBrightness(int sensorReading) {
    int i = sensorReading;
    if(i <= 1024) {
      analogWrite(ledPin, i); // changing brightness and delays here
      delay(5);               // to implement "animations"
      return calcBrightness(i+2); // calcBrightness calls itself
    }
    return 0;
  }
  ```


# Server
The client consists of an Adafruit Feather HUZZAH ESP8266 and an photoresistor as sensor. The sensor is connected the analog-to-digital converter (ADC) of the board. The ADC measures the voltage and generates a value between 0 - 1023.
Whenever a client sends a GET request the server will answer by sending some data. The following handlers are implemented:

| URL                 | Data sent      |
| ------------------- | -------------- |
| ServerIP/sensor     | sensor reading | 
| ServerIP/brightness | sensor reading or override value, depending on checkbox on / |
| ServerIP/           | a web interface to enter override |

So if the server recieves a GET request with /sensor or /brightness will return a value as plain text.

If it recieves a request to root it returns some html of a webinterface by a browser. This html code also contains three values:
  * the current sensor reading (we get that directly from the pin at the moment the handler is called)
  * the setting if an override should be sent (represented by a checkbox)
  * the value for an override

    ![](server/screenshot_webinterface.png)

  The latter two can be set by the client via arguments. Notice how in the screenshot it says `?over=on&overVal=905` behind the URL. This means the client is sending the argument `over` (meaning override) with the value `on` and the argument `overVal` with the value `905`.

  The server will use these values as it's new settings for override.

## Hardware
![](server/server_sketch.png)

## Configuration
* Enter credentials for your WiFi
  ```c
  // WiFi Credentials
  const char* ssid = "myWifiSSID";    // name of the network
  const char* password = "myWifiKey"; // WiFi key
  ```
* If you use a different board than the Adafruit Feather HUZZAH ESP8266 make sure to have the ADC pin number set
  ```c
  const int sensorPin = 0; // the pin that the photoresistor is connected to
  ```  
* The IP address will usually be automatically assigned by your Access Point. To find out you servers address you could [scan your network](https://askubuntu.com/a/224567), check the DHCP table of your AP or connect the server to a computer via USB, open a [serial monitor](https://www.arduino.cc/en/Guide/Environment#toc12). You should get a message like this:
  ```
  Connected to YourWifi
  IP address: 192.168.0.42
  HTTP server started
  ```
* You can connect to http://ServerIP with a web browser of any device in the same network. There will be a small interface that shows the sensor reading at the moment of loading the page. You also can set an override for /brightness.


## Logging Data on SD card with Adalogger
[Adalogger](https://learn.adafruit.com/adafruit-feather-m0-adalogger/using-the-sd-card#) support hasn't been implemented yet. You will need to set `chipSelect = 15;`

# License
* Icons in [concept drawing](misc/concept-drawing.jpg) made by [Flat Icon](https://www.flaticon.com/authors/flat-icons) and [Freepik](https://www.flaticon.com/authors/freepik) from www.flaticon.com
* The rest is made by the PAN-PROJECT Team and falls under [MIT License](LICENSE)