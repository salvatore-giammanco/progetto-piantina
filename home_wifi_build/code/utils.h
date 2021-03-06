#ifndef _UTILS_H_
#define _UTILS_H_

#include "memory.h"

//Function to map a value on a range
float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Function to print a value p on serial, along with a final value f which also generates a new line
template<typename T>
void print_raw_reading(T p){
	logger.log(String(millis())+": "+String(p));
}

//Function to sample the soil moisture
//It performs NumberReadings number of readings of the soil moisture, at times ReadingsInt
//The final moisture sample value is the average of all the readings
//The sampled values are converted into percentages based on AirValue and WaterValue
float read_soil_moisture(){
	// Convert the analog read into voltage
	// ESP32 returns a value from 0 (0V) to 4095 (3.3V)
	float reading = ((float) analogRead(MOISTURE_READING_PIN)/4095.0)*3.3;
	logger.log("Analog read: "+String(reading));
	return reading;
}

float soil_moisture_percent(float soilMoistureAverage){
	float soilMoisturePercent = map_range(soilMoistureAverage, airValue.value, waterValue.value, 0.0, 100.0);
	return constrain(soilMoisturePercent, 0.0, 100.0); //Returns the percentage of moisture
}

void read_soil_moisture_percent_average(){
	float soilMoistureAverage = 0;
	// Read the moisture NumberReadings times, separated by ReadingsIntervalMS milliseconds
	for(int i=0;i<NumReadings;i++){
		soilMoistureAverage += ((float)read_soil_moisture()/NumReadings); //Average of all the readings
		delay(ReadingsInt); //Pause between two readings
	}
	soilMoisture.raw_value = soilMoistureAverage;
	soilMoisture.value = soil_moisture_percent(soilMoistureAverage);
	print_raw_reading(soilMoisture.value); //Prints the average value
}

#endif
