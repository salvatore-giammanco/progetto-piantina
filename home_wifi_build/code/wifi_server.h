#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include "memory.h"
#include "utils.h"
#include <WiFi.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include <ArduinoJson.h>

StaticJsonDocument<250> sensorJson;
char buffer[250];
esp_mqtt_client_config_t mqtt_cfg;
esp_mqtt_client_handle_t client;

template<typename T>
void createJson(T value) {
	sensorJson.clear();
	sensorJson["value"] = value;
	serializeJson(sensorJson, buffer);
}

String convert_to_string(char* text, int len){
	String out = "";
	for(char* l=text; l!=(text+len);l++){
		out+=*l;
	}
	return out;
}

static esp_err_t messageHandler(esp_mqtt_event_handle_t event){
	String event_topic = convert_to_string(event->topic, event->topic_len);
	String event_data = convert_to_string(event->data, event->data_len);
	preferences.begin(variablesNamespace, false);
	if(event_topic==pumpOverride.commandTopic){
		Serial.println("Received command for Pump Override");
		if (event_data == "on") {
			Serial.println("Turning Pump Override on");
			pumpOverride.value = true;
			esp_mqtt_client_publish(client, pumpOverride.stateTopic.c_str(), "on", 2, 1, 1);
		}
		else if (event_data == "off") {
			Serial.println("Turning Pump Override off");
			pumpOverride.value = false;
			esp_mqtt_client_publish(client, pumpOverride.stateTopic.c_str(), "off", 3, 1, 1);
		}
		updateVar(pumpOverride.value, pumpOverride.key);
	}
	else if(event_topic==pumpSwitch.commandTopic){
		Serial.println("Received command for Pump Switch");
		if (event_data == "on") {
			Serial.println("Turning Pump Switch on");
			pumpSwitch.value = true;
			esp_mqtt_client_publish(client, pumpSwitch.stateTopic.c_str(), "on", 2, 1, 1);
		}
		else if (event_data == "off") {
			Serial.println("Turning Pump Switch off");
			pumpSwitch.value = false;
			esp_mqtt_client_publish(client, pumpSwitch.stateTopic.c_str(), "off", 3, 1, 1);
		}
		updateVar(pumpSwitch.value, pumpSwitch.key);
	}
	else if(event_topic==moistureTresh.stateTopic){
		Serial.println("Received moisture_tresh");
		moistureTresh.value = event_data.toFloat();
		Serial.println("moisture_tresh value: "+String(moistureTresh.value));
		updateVar(moistureTresh.value, moistureTresh.key);
	}
	else if(event_topic==airValue.stateTopic){
		Serial.println("Received air_value");
		airValue.value = event_data.toInt();
		Serial.println("air_value value: "+String(airValue.value));
		updateVar(airValue.value, airValue.key);
	}
	else if(event_topic==waterValue.stateTopic){
		Serial.println("Received water_value");
		waterValue.value = event_data.toInt();
		Serial.println("water_value value: "+String(waterValue.value));
		updateVar(waterValue.value, waterValue.key);
	}
	else if(event_topic==samplingTime.stateTopic){
		Serial.println("Received sampling_time");
		samplingTime.value = event_data.toInt();
		Serial.println("sampling_time value: "+String(samplingTime.value));
		updateVar(samplingTime.value, samplingTime.key);
	}
	else if(event_topic==pumpRuntime.stateTopic){
		Serial.println("Received pump_runtime");
		pumpRuntime.value = event_data.toInt();
		Serial.println("pump_runtime value: "+String(pumpRuntime.value));
		updateVar(pumpRuntime.value, pumpRuntime.key);
	}
	else if(event_topic==wateringTime.stateTopic){
	    Serial.println("Received watering_time");
	    wateringTime.value = event_data.toInt();
	    Serial.println("watering_time value: "+String(wateringTime.value));
		updateVar(wateringTime.value, wateringTime.key);
	}
	else{
		Serial.println("Received unknown topic "+event_topic);
	}
	preferences.end();
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
	if(event->event_id == MQTT_EVENT_CONNECTED) {
		Serial.println("Connection established");
		Serial.println("Fetching variables from the broker.");
		// Send an on state to mean the pump/board has started and is connected
		esp_mqtt_client_publish(client, pump.availabilityTopic.c_str(), "on", 2, 1, 0);
		esp_mqtt_client_publish(client, soilMoisture.availabilityTopic.c_str(), "on", 2, 1, 0);
		esp_mqtt_client_publish(client, pump.stateTopic.c_str(), "off", 3, 1, 1);
		// Subscriptions
		esp_mqtt_client_subscribe(client, pumpOverride.commandTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, pumpSwitch.commandTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, moistureTresh.stateTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, airValue.stateTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, waterValue.stateTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, samplingTime.stateTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, pumpRuntime.stateTopic.c_str(), 1);
		esp_mqtt_client_subscribe(client, wateringTime.stateTopic.c_str(), 1);
		Serial.println("Subscribed to all topics");
	} else if(event->event_id == MQTT_EVENT_DISCONNECTED) {
		ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_DISCONNECTED", event->event_id);
		//esp_mqtt_client_reconnect (event->client); //not needed if autoconnect is enabled
	} else  if(event->event_id == MQTT_EVENT_SUBSCRIBED) {
		ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_SUBSCRIBED", event->msg_id, event->event_id);
	} else  if(event->event_id == MQTT_EVENT_UNSUBSCRIBED) {
		ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_UNSUBSCRIBED", event->msg_id, event->event_id);
	} else  if(event->event_id == MQTT_EVENT_PUBLISHED) {
		ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_PUBLISHED", event->event_id);
		String event_topic = convert_to_string(event->topic, event->topic_len);
		String event_data = convert_to_string(event->data, event->data_len);
    	Serial.println("Published message "+event_data+" to topic "+event_topic);
	} else  if(event->event_id == MQTT_EVENT_DATA) {
		ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_DATA", event->msg_id, event->event_id);
		ESP_LOGI("TEST", "Topic length %d. Data length %d", event->topic_len, event->data_len);
		ESP_LOGI("TEST","Incoming data: %.*s %.*s\n", event->topic_len, event->topic, event->data_len, event->data);
		Serial.println("Received message");
    	messageHandler(event);
	} else if(event->event_id == MQTT_EVENT_BEFORE_CONNECT) {
		ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_BEFORE_CONNECT", event->event_id);
	} else if(event->event_id == MQTT_EVENT_ERROR){
		if(event->error_handle->connect_return_code != MQTT_CONNECTION_ACCEPTED){
			Serial.println("Couldn't connect to the broker, fetching vars from memory.");
			getAllFromMemory();
		} else {
			Serial.println("Unknown error!");
		}
	}
	return ESP_OK;
}

void client_setup(){
	mqtt_cfg.host = MQTTBrokerIP;
	mqtt_cfg.port = MQTTBrokerPort;
	mqtt_cfg.username = MQTTUser;
	mqtt_cfg.password = MQTTPass;
	mqtt_cfg.keepalive = 15;
	mqtt_cfg.transport = MQTT_TRANSPORT_OVER_SSL;
	mqtt_cfg.event_handle = mqtt_event_handler;
	mqtt_cfg.lwt_topic = "debug";
	mqtt_cfg.lwt_msg = "0";
	mqtt_cfg.lwt_msg_len = 1;
	WiFi.mode(WIFI_MODE_STA);
}

void wifi_mqtt_connect(){
	client_setup();
	WiFi.begin(SSID, PASS);
	while(!WiFi.isConnected()) {
		Serial.print('.');
		delay(100);
	}
	Serial.println();
	Serial.print("Connected: ");
	Serial.println(WiFi.localIP());
	esp_err_t err = esp_tls_set_global_ca_store(DSTroot_CA, sizeof(DSTroot_CA));
	client = esp_mqtt_client_init(&mqtt_cfg);
	err = esp_mqtt_client_start(client);
	Serial.println("MQTT started");
}

void wifi_mqtt_disconnect(){
	esp_mqtt_client_disconnect(client);
	WiFi.disconnect();
	Serial.println("Disconnecting");
	while(WiFi.isConnected()){
		Serial.print('.');
		delay(100);
	}
}

#endif
