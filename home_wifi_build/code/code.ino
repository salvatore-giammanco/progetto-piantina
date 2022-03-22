#include "wifi_server.h"
#include <time.h>

void setup() {
  setCpuFrequencyMhz(80);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  Serial.begin(115200);
  while(!Serial);
  Serial.println("MQTT client loop: "+String(boot++));
  while((!client.isConnected()) && (millis()<10000)){
    client.loop();
    Serial.print(".");
    delay(500);
  }
  // Start preferences
  preferences.begin(variablesNamespace, false);
  if(client.isConnected()){
    // Send an on state to mean the pump/board has started and is connected
    client.publish(pump.availabilityTopic, "on");
    client.publish(soilMoisture.availabilityTopic, "on");
    client.publish(pump.stateTopic, "off");
    // Run loop to update all variables from the broker
    for(int i=0;i<100;i++){
      client.loop();
      delay(100);
    }
    // Update variables in memory
    updateVar(pumpOverride.value, pumpOverride.key);
    updateVar(pumpSwitch.value, pumpSwitch.key);
    updateVar(moistureTresh.value, moistureTresh.key);
    updateVar(airValue.value, airValue.key);
    updateVar(waterValue.value, waterValue.key);
    updateVar(samplingTime.value, samplingTime.key);
    updateVar(pumpRuntime.value, pumpRuntime.key);
    updateVar(wateringTime.value, wateringTime.key);
  }
  else{
    // If we can't get variables from the broker, get the last ones from memory
    getAllFromMemory();
  }
  // Get the last time the pump was run
  time_t lastTime = preferences.getInt(timeVar);
  preferences.end();
  // Deep sleep timer
  Serial.println("Setting timer wakeup");
  esp_sleep_enable_timer_wakeup(samplingTime.value * s_to_us);
  // Start code to wet the plant
  Serial.println("Reading soil moisture");
  read_soil_moisture_percent_average();
  // Debug
  client.publish(soilMoisture.debugTopic, String(soilMoisture.raw_value), true);
  ///
  createJson<float>(soilMoisture.value);
  client.publish(soilMoisture.stateTopic, buffer, true);
  if(pumpOverride.value || (pumpSwitch.value && (difftime(time(NULL), lastTime) >= (wateringTime.value * 3600)) && (soilMoisture.value < moistureTresh.value))){
    Serial.println("Running pump for "+String(pumpRuntime.value)+" seconds");
    digitalWrite(PUMP_PIN, HIGH);
    client.publish(pump.stateTopic, "on");
    delay(pumpRuntime.value * sToMs);
    digitalWrite(PUMP_PIN, LOW);
    client.publish(pump.stateTopic, "off");
  }
  else{
    Serial.println("Condition to water plant not met, pump is off");
    client.publish(pump.stateTopic, "off");
  }
  delay(2000);
  // Send an off state to mean the pump/board is going to sleep
  client.publish(pump.availabilityTopic, "off");
  client.publish(soilMoisture.availabilityTopic, "off");
  // Deep sleep
  Serial.println("Going into sleep for "+String(samplingTime.value)+" seconds");
  esp_deep_sleep_start();
}

void loop() {}
