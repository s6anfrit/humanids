#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x0C, 0x2D};
byte ip[] = {192, 168, 0, 177};
EthernetServer webServer(80);
EthernetServer idsServer(90);
const int sensorPin = 0;
int sensorState = 0;
String buffer;
boolean serverState = false;

void setup() {
  pinMode(sensorPin, INPUT);
  Ethernet.begin(mac, ip);
  webServer.begin();
  idsServer.begin();
  Serial.begin(9600);
  Serial.println("Server started.");
}

void loop() {
  EthernetClient client = idsServer.available();
  if (client) {
    client.println("Hello Client.");
    while (client.connected()) {
      sensorState = analogRead(sensorPin);
      Serial.println(sensorState);
      webLoop();
      if (client.available() && sensorState == 1023 && serverState) {
        Serial.println("Alert!");
        client.println("Alert!");
        delay(1);
        client.stop();
      }
    }
  }
}

void webLoop() {
  EthernetClient client = webServer.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer += c;
        if (c == '\n') {
          if (buffer.indexOf("favicon") == -1) {
            if (buffer.indexOf("state=on") > -1 && !serverState) serverState = true;
            else if (buffer.indexOf("state=off") > -1 && serverState) serverState = false;
            (!serverState)? site(client, "on") : site(client, "off");
          }
          buffer = "";
          break;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void site(EthernetClient client, String serverState) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.print("<!DOCTYPE html><html><head><title>Arduino Server</title></head><body>");
  client.print("<h1>Arduino Server</h1><form action='/' method='GET'>");
  client.print("Turn the server <input type='submit' name='state' value='" + serverState + "'>");
  client.print("</form></body></html>");  
}
