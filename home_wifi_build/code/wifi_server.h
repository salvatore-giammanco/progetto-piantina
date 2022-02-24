#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

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


void getValueFloat(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  create_json(keyname, preferences.getFloat(keyname), unit);
  server.send(200, "application/json", buffer);
}


void getValueShort(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  create_json(keyname, preferences.getShort(keyname), unit);
  server.send(200, "application/json", buffer);
}


void getValueLong(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  create_json(keyname, preferences.getLong(keyname), unit);
  server.send(200, "application/json", buffer);
}


void changeValueFloat(char* keyname, float value = -1) {
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
  }
  Serial.print("Setting ");
  Serial.println(keyname);
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  float set_value = 0;
  if(value>0){
    Serial.println("Resetting it to its default value");
    set_value = value;
  }
  else{
    set_value = (float)jsonDocument["value"];
  }
  
  Serial.print("Setting value to");
  Serial.println(set_value);
  preferences.putFloat(keyname, set_value);
  Serial.print(keyname);
  Serial.print(" set to ");
  Serial.println((float)preferences.getFloat(keyname));
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void changeValueShort(char* keyname, short value = -1) {
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
  }
  Serial.print("Setting ");
  Serial.print(keyname);
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  short set_value = 0;
  if(value>0){
    Serial.println("Resetting it to its default value");
    set_value = value;
  }
  else{
    set_value = (short)jsonDocument["value"];
  }
  
  Serial.print("Setting value to");
  Serial.println(set_value);
  preferences.putShort(keyname, set_value);
  Serial.print(keyname);
  Serial.print(" set to ");
  Serial.println((short)preferences.getShort(keyname));
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void changeValueLong(char* keyname, long value = -1) {
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
  }
  Serial.print("Setting ");
  Serial.print(keyname);
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  long set_value = 0;
  if(value>0){
    Serial.println("Resetting it to its default value");
    set_value = value;
  }
  else{
    set_value = (long)jsonDocument["value"];
  }
  
  Serial.print("Setting value to");
  Serial.println(set_value);
  preferences.putLong(keyname, set_value);
  Serial.print(keyname);
  Serial.print(" set to ");
  Serial.println((long)preferences.getLong(keyname));
  // Respond to the client
  server.send(200, "application/json", "{}");
}


void setup_routing() {
  //Sets a server routing so that each endpoint is assigned to an handler
  //MoistureTresh
  server.on("/moisture", [](){getValueFloat("MoistureTresh", "%");});
  server.on("/moisture/set", HTTP_POST, [](){changeValueFloat("MoistureTresh");});
  server.on("/moisture/reset", HTTP_POST, [](){changeValueFloat("MoistureTresh", DefaultMoistureTresh);});
  //AirValue
  server.on("/airvalue", [](){getValueFloat("AirValue");});
  server.on("/airvalue/set", HTTP_POST, [](){changeValueFloat("AirValue");});
  server.on("/airvalue/reset", HTTP_POST, [](){changeValueFloat("AirValue", DefaultAirValue);});
  //WaterValue
  server.on("/watervalue", [](){getValueFloat("WaterValue");});
  server.on("/watervalue/set", HTTP_POST, [](){changeValueFloat("WaterValue");});
  server.on("/watervalue/reset", HTTP_POST, [](){changeValueFloat("WaterValue", DefaultWaterValue);});
  //SamplingTime
  server.on("/samplingtime", [](){getValueLong("SamplingTime");});
  server.on("/samplingtime/set", HTTP_POST, [](){changeValueLong("SamplingTime");});
  server.on("/samplingtime/reset", HTTP_POST, [](){changeValueLong("SamplingTime", DefaultSamplingTime);});
  //PumpRuntime
  server.on("/pumpruntime", [](){getValueLong("PumpRuntime");});
  server.on("/pumpruntime/set", HTTP_POST, [](){changeValueLong("PumpRuntime");});
  server.on("/pumpruntime/reset", HTTP_POST, [](){changeValueLong("PumpRuntime", DefaultPumpRuntime);});
  //ReadingsInt
  server.on("/readingsint", [](){getValueLong("ReadingsInt");});
  server.on("/readingsint/set", HTTP_POST, [](){changeValueLong("ReadingsInt");});
  server.on("/readingsint/reset", HTTP_POST, [](){changeValueLong("ReadingsInt", DefaultReadingsInt);});
  //NumReading
  server.on("/numreading", [](){getValueShort("NumReading");});
  server.on("/numreading/set", HTTP_POST, [](){changeValueShort("NumReading");});
  server.on("/numreading/reset", HTTP_POST, [](){changeValueShort("NumReading", DefaultNumReading);});
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