#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include "constants.h"
#include "memory.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


const char *SSID = "abc";
const char *PWD = "123";
// Web server running on port 80
WebServer server(80);
StaticJsonDocument<250> jsonDocument;
char buffer[250];


void create_json(char *tag, float value, char *unit) {
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}


void getValue(char* keyname) {
  Serial.print("Get ");
  Serial.println(keyname);
  create_json(keyname, preferences.getFloat(keyname), "%");
  server.send(200, "application/json", buffer);
}


void changeValue(char* keyname) {
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
  }
  Serial.println("Set moisture treshold");
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  // Get RGB components
  Serial.print("Setting moisture treshold to");
  Serial.println((float)jsonDocument["value"]);
  preferences.putFloat("MoistureTresh", jsonDocument["value"]);
  Serial.print("Moisture treshold set to ");
  Serial.println((float)preferences.getFloat("MoistureTresh"));
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void resetMoistureTresh() {
  Serial.println("Reset moisture treshold to its default value");
  preferences.putFloat("MoistureTresh", DefaultMoistureTresh);
  Serial.print("Reset moisture treshold to ");
  Serial.println(DefaultMoistureTresh);
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void setup_routing() {
  //Sets a server routing so that each endpoint is assigned to an handler
  //MoistureTresh
  server.on("/treshold", [&](){getValue("MoistureTresh");});
  server.on("/treshold/set", HTTP_POST, [](){changeValue("MoistureTresh");});
  server.on("/treshold/reset", HTTP_POST, resetMoistureTresh);
  // start server
  server.begin();
}


void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
      // we can even make the ESP32 to sleep
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}


#endif