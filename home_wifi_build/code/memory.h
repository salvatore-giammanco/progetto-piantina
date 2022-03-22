#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_

#include <Preferences.h>

Preferences preferences;
int s_to_us = 1000000;
RTC_DATA_ATTR int boot = 0;
short NumReadings = 10; // Read moisture 10 times
int ReadingsInt = 1000; // Read every 30 seconds
const char SSID[] = "SSID";    // Network SSID (name)
const char PASS[] = "PASS";    // Network password (use for WPA, or use as key for WEP)
const char MQTTBrokerIP[] = "192.168.1.84";
const char MQTTUser[] = "luigi";
const char MQTTPass[] = "pass";
const short MQTTBrokerPort = 1884;
static const int sToMs = 1000; // Conversion from Seconds to mS
static const long DefaultMS_minute = 60 * sToMs; //Milliseconds per minute
static const long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
static const int MOISTURE_READING_PIN = 34;
static const int PUMP_PIN = 33;
static const char variablesNamespace[] = "constants";
static const char timeVar[] = "time";

struct MQTTtopics {
  MQTTtopics():
    key("debug"),
    commandTopic("debug"),
    stateTopic("debug"),
    availabilityTopic("debug"),
    debugTopic("debug")
  {}

  char* key;
  char* commandTopic;
  char* stateTopic;
  char* availabilityTopic;
  char* debugTopic;
};

struct PumpOverride: MQTTtopics {
  PumpOverride():
    MQTTtopics(),
    value(false)
  {
    key = "PumpOverride";
    commandTopic = "abegghome/pump_override/com";
    stateTopic = "abegghome/pump_override/state";
  }

  bool value;
} pumpOverride;

struct PumpSwitch: MQTTtopics {
  PumpSwitch():
    MQTTtopics(),
    value(false)
  {
    key = "PumpSwitch";
    commandTopic = "abegghome/pump_switch/com";
    stateTopic = "abegghome/pump_switch/state";
  }

  bool value;
} pumpSwitch;

struct Pump: MQTTtopics {
  Pump():
    MQTTtopics()
  {
    key = "Pump";
    stateTopic = "abegghome/pump/state";
    availabilityTopic = "abegghome/pump/availability";
  }
} pump;

struct MoistureTresh: MQTTtopics {
  MoistureTresh():
    MQTTtopics(),
    value(0)
  {
    key = "MoistureTresh";
    stateTopic = "abegghome/moisture_tresh/state";
  }

  float value;
} moistureTresh;

struct SoilMoisture: MQTTtopics {
  SoilMoisture():
    MQTTtopics(),
    raw_value(0),
    value(0)
  {
    key = "SoilMoisture";
    stateTopic = "abegghome/moisture/state";
    availabilityTopic = "abegghome/moisture/availability";
    debugTopic = "abegghome/moisture/debug";
  }

  float raw_value;
  float value;
} soilMoisture;

struct AirValue: MQTTtopics {
  AirValue():
    MQTTtopics(),
    value(0)
  {
    key = "AirValue";
    stateTopic = "abegghome/air_value/state";
  }

  int value;
} airValue;

struct WaterValue: MQTTtopics {
  WaterValue():
    MQTTtopics(),
    value(0)
  {
    key = "WaterValue";
    stateTopic = "abegghome/water_value/state";
  }
  
  int value;
} waterValue;

struct SamplingTime: MQTTtopics {
  SamplingTime():
    MQTTtopics(),
    value(0)
  {
    key = "SamplingTime";
    stateTopic = "abegghome/sampling_time/state";
  }
  
  int value;
} samplingTime;

struct PumpRuntime: MQTTtopics {
  PumpRuntime():
    MQTTtopics(),
    value(0)
  {
    key = "PumpRuntime";
    stateTopic = "abegghome/pump_runtime/state";
  }
  
  int value;
} pumpRuntime;

struct WateringTime: MQTTtopics {
  WateringTime():
    MQTTtopics(),
    value(0)
  {
    key = "WateringTime";
    stateTopic = "abegghome/watering_time/state";
  }
  
  int value;
} wateringTime;

void updateVar(float value, char* keyname){
  if(value != preferences.getFloat(keyname, NAN)){
    Serial.println("updating value "+String(keyname));
    preferences.putFloat(keyname, value);
  }
}

void updateVar(int value, char* keyname){
  if(value != preferences.getInt(keyname, NAN)){
    Serial.println("updating value "+String(keyname));
    preferences.putInt(keyname, value);
  }
}

void updateVar(bool value, char* keyname){
  if(value != preferences.getBool(keyname, NAN)){
    Serial.println("updating value "+String(keyname));
    preferences.putBool(keyname, value);
  }
}

void getAllFromMemory(){
  pumpOverride.value = preferences.getBool(pumpOverride.key);
  pumpSwitch.value = preferences.getBool(pumpSwitch.key);
  moistureTresh.value = preferences.getFloat(moistureTresh.key);
  soilMoisture.value = preferences.getInt(soilMoisture.key);
  airValue.value = preferences.getInt(airValue.key);
  waterValue.value = preferences.getInt(waterValue.key);
  samplingTime.value = preferences.getInt(samplingTime.key);
  pumpRuntime.value = preferences.getInt(pumpRuntime.key);
  wateringTime.value = preferences.getInt(wateringTime.key);
}

#endif
