#ifndef _UTILS_H_
#define _UTILS_H_

#include "memory.h"

//Function to map a value on a range
float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Function to print a value p on serial, along with a final value f which also generates a new line
template<typename T>
void print_on_serial(T p){
  Serial.print(millis());
  Serial.print(": ");
  Serial.println(p);
}


//Function to sample the soil moisture
//It performs NumberReadings number of readings of the soil moisture, at times ReadingsInt
//The final moisture sample value is the average of all the readings
//The sampled values are converted into percentages based on AirValue and WaterValue
float read_soil_moisture(){
  return (float)analogRead(MOISTURE_READING_PIN);
}


float read_soil_moisture_percent(){
  float AirValue = preferences.getFloat("AirValue");
  float WaterValue = preferences.getFloat("WaterValue");
  float SoilMoisture = read_soil_moisture();
  float SoilMoisturePercent = map_range(SoilMoisture, AirValue, WaterValue, 0.0, 100.0);
  return constrain(SoilMoisturePercent, 0.0, 100.0); //Returns the percentage of moisture
}

float read_soil_moisture_percent_average(){
  float SoilMoisturePercentAverage = 0;
  short NumReadings = preferences.getShort("NumReadings");
  long ReadingsInt = preferences.getLong("ReadingsInt");
  // Read the moisture NumberReadings times, separated by ReadingsIntervalMS milliseconds
  for(int i=0;i<NumReadings;i++){
    SoilMoisturePercentAverage += read_soil_moisture_percent()/NumReadings; //Average of all the readings
    delay(ReadingsInt); //Pause between two readings
  }
  print_on_serial(SoilMoisturePercentAverage); //Prints the average value
  return SoilMoisturePercentAverage;
}

#endif
