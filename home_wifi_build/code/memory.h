#ifndef _READ_MEMORY_H_
#define _READ_MEMORY_H_

#include <Preferences.h>
#include "mqtt_client.h"

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
static const int MOISTURE_READING_PIN = 36;
static const int PUMP_PIN = 26;
static const gpio_num_t FULL_SWITCH = GPIO_NUM_25;
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

struct Logger: MQTTtopics {
	bool serial;
	bool mqttConnected;
	esp_mqtt_client_handle_t client;
	
	Logger(bool serial):
		MQTTtopics()
	{
		this->serial = serial;
		this->mqttConnected = false;
		this->client = nullptr;
		stateTopic = "abegghome/logger/state";
	}

	void log(String msg, bool newline = true){
		if(this->serial){
			(newline?Serial.println(msg):Serial.print(msg));
		}
		else{
			if(this->mqttConnected){
				esp_mqtt_client_publish(this->client, this->stateTopic.c_str(), msg.c_str(), 0, 1, 1);
			} else {
				(newline?Serial.println(msg):Serial.print(msg));
			}
		}
	}
} logger(false);

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
	preferences.begin(variablesNamespace, false);
	if(value != preferences.getFloat(keyname.c_str(), NAN)){
		logger.log("updating value "+keyname);
		preferences.putFloat(keyname.c_str(), value);
	}
	preferences.end();
}

void updateVar(int value, String keyname){
	preferences.begin(variablesNamespace, false);
	if(value != preferences.getInt(keyname.c_str(), NAN)){
		logger.log("updating value "+keyname);
		preferences.putInt(keyname.c_str(), value);
	}
	preferences.end();
}

void updateVar(bool value, String keyname){
	preferences.begin(variablesNamespace, false);
	if(value != preferences.getBool(keyname.c_str(), NAN)){
		logger.log("updating value "+keyname);
		preferences.putBool(keyname.c_str(), value);
	}
	preferences.end();
}

void getAllFromMemory(){
	logger.log("Getting variables from memory");
	preferences.begin(variablesNamespace, false);
	pumpOverride.value = preferences.getBool(pumpOverride.key.c_str());
	logger.log("pumpOverride value: "+String(pumpOverride.value));
	pumpSwitch.value = preferences.getBool(pumpSwitch.key.c_str());
	logger.log("pumpSwitch value: "+String(pumpSwitch.value));
	moistureTresh.value = preferences.getFloat(moistureTresh.key.c_str());
	logger.log("moistureTresh value: "+String(moistureTresh.value));
	soilMoisture.value = preferences.getInt(soilMoisture.key.c_str());
	logger.log("soilMoisture value: "+String(soilMoisture.value));
	airValue.value = preferences.getFloat(airValue.key.c_str());
	logger.log("airValue value: "+String(airValue.value));
	waterValue.value = preferences.getFloat(waterValue.key.c_str());
	logger.log("waterValue value: "+String(waterValue.value));
	samplingTime.value = preferences.getInt(samplingTime.key.c_str());
	logger.log("samplingTime value: "+String(samplingTime.value));
	pumpRuntime.value = preferences.getInt(pumpRuntime.key.c_str());
	logger.log("pumpRuntime value: "+String(pumpRuntime.value));
	wateringTime.value = preferences.getInt(wateringTime.key.c_str());
	logger.log("wateringTime value: "+String(wateringTime.value));
	preferences.end();
}

#endif
