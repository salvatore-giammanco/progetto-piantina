#ifndef _UTILS_H_
#define _UTILS_H_

#include "memory.h"

//Function to map a value on a range
float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Function to print a value p on serial, along with a final value f which also generates a new line
template<typename T, typename F>
void print_on_serial(T p, F fin){
  Serial.println();
  Serial.print(millis());
  Serial.print(": ");
  Serial.print(p);
  Serial.println(fin);
}


//Function to sample the soil moisture
//It performs NumberReadings number of readings of the soil moisture, at times ReadingsInt
//The final moisture sample value is the average of all the readings
//The sampled values are converted into percentages based on AirValue and WaterValue
float read_soil_moisture_percent(){
  float AirValue = preferences.getFloat("AirValue");
  float WaterValue = preferences.getFloat("WaterValue");
  float SoilMoisture = (float) analogRead(MOISTURE_READING_PIN);
  float SoilMoisturePercent = map_range(SoilMoisture, AirValue, WaterValue, 0.0, 100.0);
  print_on_serial('\r', SoilMoisturePercent); //Prints the absolute value
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
  print_on_serial('\n', SoilMoisturePercentAverage); //Prints the average value
  return SoilMoisturePercentAverage;
}

#endif