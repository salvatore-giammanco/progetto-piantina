#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_


#include <Preferences.h>


Preferences preferences;
static const float DefaultMoistureTresh = 50.0; //Moisture percentage treshold
static const float DefaultAirValue = 470.0; //Value the moisture sensor gives when dry
static const float DefaultWaterValue = 222.0; //Value the moisture sensor gives when submerged in water
static const long DefaultMS_minute = 60000; //Milliseconds per minute
static const long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
static const long DefaultSamplingTime = DefaultMS_hour; //Time between two consecutive samplings of the soil mosture
static const long DefaultPumpRuntime = 5*DefaultMS_minute; //Time the pump runs when activated
static const long DefaultReadingsInt = DefaultMS_minute/2; //Time between two consecutive moisture readings for a sampling
static const short DefaultNumReadings = 10; //Number of moisture readings for each sampling
static const int MOISTURE_READING_PIN = 34;
static const int RELAY_PIN = 23;


void readSetVar(float value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getFloat(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.putFloat(keyname, value);
  }
}


void readSetVar(long value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getLong(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.putLong(keyname, value);
  }
}


void readSetVar(short value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getShort(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.putShort(keyname, value);
  }
}

#endif
