#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_

#include <Preferences.h>

Preferences preferences;
static const int sToUs = 1000000;
static const int connectionTimeoutSeconds = 10;
RTC_DATA_ATTR int boot = 0;
static const short NumReadings = 10; // Read moisture 10 times
static const int ReadingsInt = 1000; // Read every 30 seconds
static const char SSID[] = "ssid";    // Network SSID (name)
static const char PASS[] = "pass";    // Network password (use for WPA, or use as key for WEP)
static const char MQTTBrokerIP[] = "192.168.1.84";
static const char MQTTUser[] = "luigi";
static const char MQTTPass[] = "pass";
static const short MQTTBrokerPort = 8883;
static const int sToMs = 1000; // Conversion from Seconds to mS
static const long DefaultMS_minute = 60 * sToMs; //Milliseconds per minute
static const long DefaultMS_hour = (long)DefaultMS_minute*60; //Milliseconds per hour
static const int MOISTURE_READING_PIN = 34;
static const int PUMP_PIN = 33;
static const char variablesNamespace[] = "constants";
static const char timeVar[] = "time";
static const unsigned char DSTroot_CA[] PROGMEM = R"EOF(
COPY HERE THE CONTENT OF CA.CRT
)EOF";



struct MQTTtopics {
	MQTTtopics():
		key("debug"),
		commandTopic("debug"),
		stateTopic("debug"),
		availabilityTopic("debug"),
		debugTopic("debug")
	{}

	String key;
	String commandTopic;
	String stateTopic;
	String availabilityTopic;
	String debugTopic;
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

	float value;
} airValue;

struct WaterValue: MQTTtopics {
	WaterValue():
		MQTTtopics(),
		value(0)
	{
		key = "WaterValue";
		stateTopic = "abegghome/water_value/state";
	}

	float value;
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

void updateVar(float value, String keyname){
	if(value != preferences.getFloat(keyname.c_str(), NAN)){
		Serial.println("updating value "+keyname);
		preferences.putFloat(keyname.c_str(), value);
	}
}

void updateVar(int value, String keyname){
	if(value != preferences.getInt(keyname.c_str(), NAN)){
		Serial.println("updating value "+keyname);
		preferences.putInt(keyname.c_str(), value);
	}
}

void updateVar(bool value, String keyname){
	if(value != preferences.getBool(keyname.c_str(), NAN)){
		Serial.println("updating value "+keyname);
		preferences.putBool(keyname.c_str(), value);
	}
}

void getAllFromMemory(){
	preferences.begin(variablesNamespace, false);
	pumpOverride.value = preferences.getBool(pumpOverride.key.c_str());
	pumpSwitch.value = preferences.getBool(pumpSwitch.key.c_str());
	moistureTresh.value = preferences.getFloat(moistureTresh.key.c_str());
	soilMoisture.value = preferences.getInt(soilMoisture.key.c_str());
	airValue.value = preferences.getFloat(airValue.key.c_str());
	waterValue.value = preferences.getFloat(waterValue.key.c_str());
	samplingTime.value = preferences.getInt(samplingTime.key.c_str());
	pumpRuntime.value = preferences.getInt(pumpRuntime.key.c_str());
	wateringTime.value = preferences.getInt(wateringTime.key.c_str());
	preferences.end();
}

#endif
