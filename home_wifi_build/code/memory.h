#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_

#include <Preferences.h>

Preferences preferences;

void read_var(float value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getFloat(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.clear();
    preferences.putFloat(keyname, value);
  }
}


void read_var(unsigned long value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getULong(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.clear();
    preferences.putULong(keyname, value);
  }
}


void read_var(unsigned short value, char* keyname){
  if(preferences.isKey(keyname)){
    Serial.print(keyname);
    Serial.println(" key found");
    Serial.println(preferences.getUShort(keyname));
  }
  else{
    Serial.print(keyname);
    Serial.println(" key not found");
    preferences.clear();
    preferences.putUShort(keyname, value);
  }
}

#endif