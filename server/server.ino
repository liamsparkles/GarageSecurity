/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

/*
    This
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Andrew-Gu"
#define STAPSK  "WelcomeHome"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(6789);
enum state {Idle, Alarm};
enum state mystate;

void setup() {
  Serial.begin(115200);

  // prepare LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
  mystate = Idle;
}

void loop() {
  // Check if a client has connected
  while (true)
  {
    delay(10);
    mystate = readBlutoothAndMotion();
    sendDataToClient(mystate);
    if (mystate == Idle) {
      idlefunction();
    }
    else if (mystate == Alarm) {
      alarmfunction();
    }
    else {
      Serial.println("umm what? That shouldn't be possible");
    }

  }
}

void sendDataToClient(enum state curstate) {
  WiFiClient client = server.available();
  if (!client) {
    Serial.println("No client");
    return;
  }
  Serial.println(F("new client"));
  client.setTimeout(5000); // default is 1000
  if (curstate == Idle) {
    client.print("idle\r");
    Serial.println("Sent state: idle\\r");
  }
  else if (curstate == Alarm) {
    client.print("alarm\r");
    Serial.println("Sent state: alarm\\r");
  }
  String recv = client.readStringUntil('\r');
  Serial.print("Response from Client: ");
  Serial.println(recv);
  Serial.println(F("Disconnecting from client"));
}

enum state readBlutoothAndMotion() {
  Serial.println("Reading Blutooth");
  return Alarm;
}

void idlefunction() {
  Serial.println("Running idle function steps.");
  //Do nothing
  return;
}

void alarmfunction() {
  Serial.println("Running alarm function steps.");
  //Do nothing
  return;
}



// Read the first line of the request
//    String req = client.readStringUntil('\r');
//    Serial.println(F("request: "));
//    Serial.println(req);

// Match the request
//    int val;
//    if (req.indexOf(F("/gpio/0")) != -1) {
//      val = 0;
//    } else if (req.indexOf(F("/gpio/1")) != -1) {
//      val = 1;
//    } else {
//      Serial.println(F("invalid request"));
//      val = digitalRead(LED_BUILTIN);
//    }

// Set LED according to the request
//    digitalWrite(LED_BUILTIN, val);

// read/ignore the rest of the request
// do not client.flush(): it is for output only, see below
//    while (client.available()) {
// byte by byte is not very efficient
//      client.read();
//    }

// Send the response to the client
// it is OK for multiple small client.print/write,
// because nagle algorithm will group them into one single packet

// The client will actually be *flushed* then disconnected
// when the function returns and 'client' object is destroyed (out-of-scope)
// flush = ensure written data are received by the other side
