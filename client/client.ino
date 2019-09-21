/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <ESP8266WiFi.h>
//#include <WiFi.h>

const char* ssid     = "Andrew-Gu";
const char* password = "WelcomeHome";
const char* host = "192.168.1.42";
const uint16_t port = 6789;

enum state{Idle, Alarm, Error};
enum state mystate;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    mystate = Idle;
}

void loop(){
  // Use WiFiClient class to create TCP connections
  while (true){
    mystate = readServer();
    if (mystate == Idle) {
      idle();
    }
    else if (mystate == Alarm) {
      alarm();
    }
    else if (mystate == Error) {
      Serial.println("Many bads");
    }
  }
}

enum state readServer() {
  enum state curstate = Error;
  WiFiClient client;
  Serial.println("Trying to connect");
  while (!client.connect(host, port)) {
    Serial.println("connection failing");
    delay(100);
  }
  Serial.println("Connected");
  if(client.connected()) {
    //unsigned long timeout = millis();
    Serial.println("Waiting for data from server");
//        client.println("test\r");
//        delay(5);
    String action = client.readStringUntil('\r');
    Serial.print("Received: ");
    Serial.print(action);
    if (action == "idle") {
      curstate = Idle;
    }
    else if (action == "alarm") {
      curstate = Alarm;
    }
    else {
      Serial.println("so many bads");
      curstate = Error;
    }
    //Assign curstate here
    client.println("received\r");
    while (client.available()) {
      Serial.println("Something went wrong");
      curstate = Error;
//          char c = client.read();
//          Serial.print(c);
    // This will send a string to the server
    }
    Serial.println();
    Serial.println("Disconnecting");
    client.stop();
    return curstate;
  }
}

void idle(){
  Serial.println("Running Idle Steps.");
  //Listen to server
  //Turn off strobe and siren
}

void alarm(){
  Serial.println("Running Alarm Steps.");
  //Strobe and siren
}
