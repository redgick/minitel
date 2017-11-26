#include <ESP8266WiFi.h>          // https://github.com/esp8266/Arduino

// needed for library
#include "WiFiManager.h"         // https://github.com/tzapu/WiFiManager

std::unique_ptr<ESP8266WebServer> server;

void handleRoot() {
  String message = "Hello minitel 2.0\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void setup() {
  Serial.begin(4800, SERIAL_7E1);

  // WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("MinitelAP");

  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  server->on("/", handleRoot);

  server->on("/minitel", []() {
    String message = server->arg(0);
    Serial.print(server->arg(0));
    Serial.write("\r\n");
    server->send(200, "text/plain", message);
  });

  server->onNotFound(handleNotFound);

  server->begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  server->handleClient();
}
