#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

const float DefaultMoistureTresh = 50.0; //Moisture percentage treshold
const float DefaultAirValue = 470.0; //Value the moisture sensor gives when dry
const float DefaultWaterValue = 222.0; //Value the moisture sensor gives when submerged in water
const float DefaultMoistureTresh = 50.0; //Moisture percentage treshold
const unsigned long DefaultMS_minute = 60000; //Milliseconds per minute
const unsigned long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
const unsigned long DefaultSamplingTime = DefaultMS_hour; //Time between two consecutive samplings of the soil mosture
const unsigned long DefaultPumpRuntime = 5*DefaultMS_minute; //Time the pump runs when activated
const unsigned long DefaultReadingsInt = DefaultMS_minute/2; //Time between two consecutive moisture readings for a sampling
const unsigned short DefaultNumReading = 10; //Number of moisture readings for each sampling
const int MOISTURE_READING_PIN = 34;
const int RELAY_PIN = 23;

#endif