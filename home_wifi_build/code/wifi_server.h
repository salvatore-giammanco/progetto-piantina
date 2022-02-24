#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include "memory.h"
#include "utils.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


const char *SSID = "abc";
const char *PWD = "123";
// Web server running on port 80
WebServer server(80);
StaticJsonDocument<250> jsonDocument;
char buffer[250];


void createJson(char *tag, float value, char *unit) {
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}


void read_moisture_api(){
  Serial.println("Get Moisture");
  float moisture = read_soil_moisture_percent();
  print_on_serial(moisture);
  createJson("Moisture", moisture, "%");
  server.send(200, "application/json", buffer);
}


void read_moisture_api_raw(){
  Serial.println("Get Moisture Absolute");
  float moisture = read_soil_moisture();
  print_on_serial(moisture);
  createJson("Moisture", moisture, "");
  server.send(200, "application/json", buffer);
}


void getValueFloat(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  createJson(keyname, preferences.getFloat(keyname), unit);
  server.send(200, "application/json", buffer);
}


void getValueShort(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  createJson(keyname, preferences.getShort(keyname), unit);
  server.send(200, "application/json", buffer);
}


void getValueLong(char* keyname, char* unit = "") {
  Serial.print("Get ");
  Serial.println(keyname);
  createJson(keyname, preferences.getLong(keyname), unit);
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


void resetAll(){
  changeValueFloat("MoistureTresh", DefaultMoistureTresh);
  changeValueFloat("AirValue", DefaultAirValue);
  changeValueFloat("WaterValue", DefaultWaterValue);
  changeValueLong("SamplingTime", DefaultSamplingTime);
  changeValueLong("PumpRuntime", DefaultPumpRuntime);
  changeValueLong("PumpRuntime", DefaultPumpRuntime);
  changeValueLong("ReadingsInt", DefaultReadingsInt);
  changeValueShort("NumReadings", DefaultNumReadings);
}


void setupRouting() {
  //Sets a server routing so that each endpoint is assigned to an handler
  //MoistureTresh
  server.on("/moisturetresh", [](){getValueFloat("MoistureTresh", "%");});
  server.on("/moisturetresh/set", HTTP_POST, [](){changeValueFloat("MoistureTresh");});
  server.on("/moisturetresh/reset", HTTP_POST, [](){changeValueFloat("MoistureTresh", DefaultMoistureTresh);});
  //AirValue
  server.on("/airvalue", [](){getValueFloat("AirValue");});
  server.on("/airvalue/set", HTTP_POST, [](){changeValueFloat("AirValue");});
  server.on("/airvalue/reset", HTTP_POST, [](){changeValueFloat("AirValue", DefaultAirValue);});
  //WaterValue
  server.on("/watervalue", [](){getValueFloat("WaterValue");});
  server.on("/watervalue/set", HTTP_POST, [](){changeValueFloat("WaterValue");});
  server.on("/watervalue/reset", HTTP_POST, [](){changeValueFloat("WaterValue", DefaultWaterValue);});
  //SamplingTime
  server.on("/samplingtime", [](){getValueLong("SamplingTime", "ms");});
  server.on("/samplingtime/set", HTTP_POST, [](){changeValueLong("SamplingTime");});
  server.on("/samplingtime/reset", HTTP_POST, [](){changeValueLong("SamplingTime", DefaultSamplingTime);});
  //PumpRuntime
  server.on("/pumpruntime", [](){getValueLong("PumpRuntime", "ms");});
  server.on("/pumpruntime/set", HTTP_POST, [](){changeValueLong("PumpRuntime");});
  server.on("/pumpruntime/reset", HTTP_POST, [](){changeValueLong("PumpRuntime", DefaultPumpRuntime);});
  //ReadingsInt
  server.on("/readingsint", [](){getValueLong("ReadingsInt", "ms");});
  server.on("/readingsint/set", HTTP_POST, [](){changeValueLong("ReadingsInt");});
  server.on("/readingsint/reset", HTTP_POST, [](){changeValueLong("ReadingsInt", DefaultReadingsInt);});
  //NumReading
  server.on("/numreadings", [](){getValueShort("NumReadings");});
  server.on("/numreadings/set", HTTP_POST, [](){changeValueShort("NumReadings");});
  server.on("/numreadings/reset", HTTP_POST, [](){changeValueShort("NumReadings", DefaultNumReadings);});
  // Get current moisture
  server.on("/moisture", read_moisture_api);
  server.on("/moisture/raw", read_moisture_api_raw);
  // Reset all variables
  server.on("/all/reset", resetAll);
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