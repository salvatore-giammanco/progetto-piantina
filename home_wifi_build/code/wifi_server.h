#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include "memory.h"
#include "utils.h"
#include <WiFi.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "a5cb482c-d6a0-4788-ab4e-90209d6bd475";
const char DEVICE_KEY[]  = "S7FXNBGKAOSCIVDOJASW";    // Secret device password
const char SSID[] = "FASTWEB-M4TDm4-2.4GHZ";    // Network SSID (name)
const char PASS[] = "6j6dP38v22";    // Network password (use for WPA, or use as key for WEP)
bool synced = false;
float soilMoisturePercentAverage;
float moistureTresh; //Moisture percentage treshold
float waterValue; //Value the moisture sensor gives when submerged in water
int samplingTime; //Seconds between two consecutive samplings of the soil mosture
int pumpRuntime; //Seconds the pump runs when activated

void updateOnSync(){
  Serial.println("Thing Properties synchronised");
  synced = true;
}

void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(soilMoisturePercentAverage, READWRITE, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(moistureTresh, READWRITE, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(airValue, READWRITE, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(waterValue, READWRITE, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(samplingTime, READWRITE, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(pumpRuntime, READWRITE, ON_CHANGE, NULL, 1);
  // ArduinoCloud.addProperty(led_switch, READWRITE, ON_CHANGE, onLedSwitchChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void connectToIoTCloud() {
  // Serial.print("Connecting to ");
  // Serial.println(SSID);
  
  // WiFi.begin(SSID, PWD);
  
  // while (WiFi.status() != WL_CONNECTED) {
  //     Serial.print(".");
  //     delay(500);
  //     // we can even make the ESP32 to sleep
  // }
  // Serial.print("Connected. IP: ");
  // Serial.println(WiFi.localIP());
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(1);
  ArduinoCloud.printDebugInfo();
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    ArduinoCloud.update();
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to Arduino IoT and syncing variables");
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, updateOnSync);
  while(!synced){
    ArduinoCloud.update();
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
  delay(1000);
}

#endif
