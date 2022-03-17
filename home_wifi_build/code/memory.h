#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_

#include <Preferences.h>

Preferences preferences;
static const int sToMs = 1000; // Conversion from Seconds to mS
static const long DefaultMS_minute = 60 * sToMs; //Milliseconds per minute
static const long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
static const int MOISTURE_READING_PIN = 34;
static const int RELAY_PIN = 23;
float airValue = 470.0; //Value the moisture sensor gives when dry
static const char variablesNamespace[] = "constants";


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


void readSetVar(int value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getInt(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.putInt(keyname, value);
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
