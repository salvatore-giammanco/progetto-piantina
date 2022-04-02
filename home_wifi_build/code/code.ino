#include "wifi_server.h"
#include <time.h>

void setup() {
	delay(500);
	setCpuFrequencyMhz(80);
	pinMode(PUMP_PIN, OUTPUT);
	pinMode(MOISTURE_READING_PIN, INPUT);
	digitalWrite(PUMP_PIN, LOW);
	Serial.begin(115200);
	while(!Serial);
	wifi_mqtt_connect();
	delay(2000);
	// Start preferences
	preferences.begin(variablesNamespace, false);
	// Get the last time the pump was run
	time_t lastTime = preferences.getInt(timeVar);
	preferences.end();
	// Start code to wet the plant
	Serial.println("Reading soil moisture");
	read_soil_moisture_percent_average();
	///
	createJson<float>(soilMoisture.value);
	esp_mqtt_client_publish(client, soilMoisture.stateTopic.c_str(), buffer, 0, 1, 1);
	if(pumpOverride.value || (pumpSwitch.value && (difftime(time(NULL), lastTime) >= (wateringTime.value * 3600)) && (soilMoisture.value < moistureTresh.value))){
		Serial.println("Running pump for "+String(pumpRuntime.value)+" seconds");
		digitalWrite(PUMP_PIN, HIGH);
		esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "on", 2, 1, 0);
		delay(pumpRuntime.value * sToMs);
		digitalWrite(PUMP_PIN, LOW);
		esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "off", 3, 1, 0);
	}
	else{
		Serial.println("Condition to water plant not met, pump is off");
		esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "off", 3, 1, 0);
	}
	delay(2000);
	// Send an off state to mean the pump/board is going to sleep and disconnect
	esp_mqtt_client_publish(client, pump.availabilityTopic.c_str(), "off", 3, 1, 0);
	esp_mqtt_client_publish(client, soilMoisture.availabilityTopic.c_str(), "off", 3, 1, 0);
	wifi_mqtt_disconnect();
	// Deep sleep
	Serial.println("Setting timer wakeup to "+String(samplingTime.value)+" seconds");
	esp_sleep_enable_timer_wakeup(samplingTime.value * s_to_us);
	Serial.println("Going into sleep for "+String(samplingTime.value)+" seconds");
	esp_deep_sleep_start();
}

void loop() {}
