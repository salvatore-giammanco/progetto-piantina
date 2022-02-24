#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

static const float DefaultMoistureTresh = 50.0; //Moisture percentage treshold
static const float DefaultAirValue = 470.0; //Value the moisture sensor gives when dry
static const float DefaultWaterValue = 222.0; //Value the moisture sensor gives when submerged in water
static const unsigned long DefaultMS_minute = 60000; //Milliseconds per minute
static const unsigned long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
static const unsigned long DefaultSamplingTime = DefaultMS_hour; //Time between two consecutive samplings of the soil mosture
static const unsigned long DefaultPumpRuntime = 5*DefaultMS_minute; //Time the pump runs when activated
static const unsigned long DefaultReadingsInt = DefaultMS_minute/2; //Time between two consecutive moisture readings for a sampling
static const unsigned short DefaultNumReading = 10; //Number of moisture readings for each sampling
static const int MOISTURE_READING_PIN = 34;
static const int RELAY_PIN = 23;

#endif