#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include "memory.h"
#include "utils.h"
#include <WiFi.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<250> sensorJson;
char buffer[250];

template<typename T>
void createJson(T value) {
  sensorJson.clear();
  sensorJson["value"] = value;
  serializeJson(sensorJson, buffer);
}

EspMQTTClient client(
  SSID,
  PASS,
  MQTTBrokerIP, // MQTT Broker server ip
  MQTTUser,
  MQTTPass,
  "Esp32",
  MQTTBrokerPort // The MQTT port, default to 1883. this line can be omitted
);

void onConnectionEstablished(){
  Serial.println("Connection established");
  client.subscribe(pumpOverride.commandTopic, [](const String & payload) {
    Serial.println("Received command for Pump Override");
    if (payload == "on") {
      Serial.println("Turning Pump Override on");
      pumpOverride.value = true;
      client.publish(pumpOverride.stateTopic, "on");
    }
    else if (payload == "off") {
      Serial.println("Turning Pump Override off");
      pumpOverride.value = false;
      client.publish(pumpOverride.stateTopic, "off");
    }
  }, 1);
  client.subscribe(pumpSwitch.commandTopic, [](const String & payload) {
    Serial.println("Received command for Pump Switch");
    if (payload == "on") {
      Serial.println("Turning Pump Switch on");
      pumpSwitch.value = true;
      client.publish(pumpSwitch.stateTopic, "on");
    }
    else if (payload == "off") {
      Serial.println("Turning Pump Switch off");
      pumpSwitch.value = false;
      client.publish(pumpSwitch.stateTopic, "off");
    }
  }, 1);
  client.subscribe(moistureTresh.stateTopic, [](const String & payload) {
    Serial.println("Received moisture_tresh");
    moistureTresh.value = payload.toFloat();
    Serial.println("moisture_tresh value: "+String(moistureTresh.value));
  }, 1);
  client.subscribe(airValue.stateTopic, [](const String & payload) {
    Serial.println("Received air_value");
    airValue.value = payload.toInt();
    Serial.println("air_value value: "+String(airValue.value));
  }, 1);
  client.subscribe(waterValue.stateTopic, [](const String & payload) {
    Serial.println("Received water_value");
    waterValue.value = payload.toInt();
    Serial.println("water_value value: "+String(waterValue.value));
  }, 1);
  client.subscribe(samplingTime.stateTopic, [](const String & payload) {
    Serial.println("Received sampling_time");
    samplingTime.value = payload.toInt();
    Serial.println("sampling_time value: "+String(samplingTime.value));
  }, 1);
  client.subscribe(pumpRuntime.stateTopic, [](const String & payload) {
    Serial.println("Received pump_runtime");
    pumpRuntime.value = payload.toInt();
    Serial.println("pump_runtime value: "+String(pumpRuntime.value));
  }, 1);
  client.subscribe(wateringTime.stateTopic, [](const String & payload) {
    Serial.println("Received watering_time");
    wateringTime.value = payload.toInt();
    Serial.println("watering_time value: "+String(wateringTime.value));
  }, 1);
}

#endif
