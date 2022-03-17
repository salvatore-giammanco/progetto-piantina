#include "wifi_server.h"

int s_to_us = 1000000;
RTC_DATA_ATTR int boot = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
  }
  /*
  * If the script has been run before, there's already a value for each key
  * Otherwise set the default one
  */
  connectToIoTCloud();
  // preferences.begin(variablesNamespace, false);
  Serial.println("Updating variables from cloud");
  ArduinoCloud.update();
  Serial.println("Moisture: "+String(soilMoisturePercentAverage));
  Serial.println("Moisture tresh: "+String(moistureTresh));
  Serial.println("Water: "+String(waterValue));
  Serial.println("Sampling (s): "+String(samplingTime));
  Serial.println("Pump (s): "+String(pumpRuntime));
  // readSetVar(soilMoisturePercentAverage, "SoilMoistPerc");
  // readSetVar(moistureTresh, "MoistureTresh");
  // readSetVar(airValue, "AirValue");
  // readSetVar(waterValue, "WaterValue");
  // readSetVar(samplingTime, "SamplingTime");
  // readSetVar(pumpRuntime, "PumpRuntime");
  // preferences.end();
  Serial.println("Started "+String(boot++)+" times");
  Serial.println("Setting timer wakeup");
  esp_sleep_enable_timer_wakeup(samplingTime * s_to_us);
  // Start
  Serial.println("Started");
  digitalWrite(RELAY_PIN, LOW);
  soilMoisturePercentAverage = read_soil_moisture_percent_average();
  Serial.println("Updating soil moisture to cloud");
  ArduinoCloud.update();
  if(soilMoisturePercentAverage < moistureTresh){
    digitalWrite(RELAY_PIN, HIGH);
    delay(pumpRuntime * sToMs);
  }
  Serial.println("Going to sleep for "+String(samplingTime)+" seconds");
  esp_deep_sleep_start();
}

void loop() {}
