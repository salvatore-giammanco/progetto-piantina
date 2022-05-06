#include "wifi_server.h"
#include "battery.h"
#include <time.h>

void setup() {
	delay(500);
	setCpuFrequencyMhz(80);
	pinMode(PUMP_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(MOISTURE_READING_PIN, INPUT);
	digitalWrite(PUMP_PIN, LOW);
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.begin(115200);
	while(!Serial);
	wifi_mqtt_connect();
	// Start memory
	preferences.begin(variablesNamespace, false);
	// Get the last time the pump was run
	time_t lastTime = preferences.getInt(timeVar);
	preferences.end();
	// Start code to wet the plant
	Serial.println("Reading soil moisture");
	read_soil_moisture_percent_average();
	createJson<float>(soilMoisture.value);
	if(pumpOverride.value || (pumpSwitch.value && (difftime(time(NULL), lastTime) >= (wateringTime.value * 3600)) && (soilMoisture.value < moistureTresh.value))){
		Serial.println("Running pump for "+String(pumpRuntime.value)+" seconds");
		digitalWrite(PUMP_PIN, HIGH);
		if(mqttConnected){
			esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "on", 2, 1, 0);
			delay(pumpRuntime.value * sToMs);
			digitalWrite(PUMP_PIN, LOW);
			esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "off", 3, 1, 0);
		} else {
			delay(pumpRuntime.value * sToMs);
			digitalWrite(PUMP_PIN, LOW);
		}
	}
	else{
		Serial.println("Condition to water plant not met, pump is off");
		if(mqttConnected){
			esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "off", 3, 1, 0);
		}
	}
	delay(1000);
	// Read battery voltage
	float batteryVoltage = read_battery();
	Serial.println("Battery voltage: "+String(batteryVoltage));
	if(mqttConnected){
		// Send an off state to mean the pump/board is going to sleep and disconnect
		esp_mqtt_client_publish(client, soilMoisture.stateTopic.c_str(), buffer, 0, 1, 1);
		esp_mqtt_client_publish(client, pump.availabilityTopic.c_str(), "off", 3, 1, 0);
		esp_mqtt_client_publish(client, soilMoisture.availabilityTopic.c_str(), "off", 3, 1, 0);
		//Send battery voltage
		createJson<float>(batteryVoltage);
		esp_mqtt_client_publish(client, "battery/state", buffer, 0, 1, 1);
		//disconnect
		wifi_mqtt_disconnect();
	}
	// Deep sleep
	// The following lines disable all RTC features except the timer
	esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
	//If battery voltage is too low, set deep sleep to be infinite (i.e. hibernate)
	if(batteryVoltage>CRITICALLY_LOW_BATTERY_VOLTAGE){
		Serial.println("Setting timer wakeup to "+String(samplingTime.value)+" seconds");
		esp_sleep_enable_timer_wakeup((unsigned long) samplingTime.value * sToUs);
		Serial.println("Going into sleep for "+String(samplingTime.value)+" seconds");
	}
	else{
		Serial.println("Hibernating");
	}
	esp_deep_sleep_start();
}

void loop() {}
