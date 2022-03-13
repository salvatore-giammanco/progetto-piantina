#include "wifi_server.h"


void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
  }
  preferences.begin("constants", false);
  /*
  * If the script has been run before, there's already a value for each key
  * Otherwise set the default one
  */
  readSetVar(DefaultMoistureTresh, "MoistureTresh");
  readSetVar(DefaultAirValue, "AirValue");
  readSetVar(DefaultWaterValue, "WaterValue");
  readSetVar(DefaultSamplingTime, "SamplingTime");
  readSetVar(DefaultPumpRuntime, "PumpRuntime");
  readSetVar(DefaultReadingsInt, "ReadingsInt");
  readSetVar(DefaultNumReadings, "NumReading");
  connectToWiFi();
  setupRouting();
}


void loop() {
  server.handleClient();
  digitalWrite(RELAY_PIN, LOW);
  delay(preferences.getLong("SamplingTime"));
  float Soilmoisturepercent = read_soil_moisture_percent_average();
  if(Soilmoisturepercent < preferences.getFloat("MoistureTresh")){
    digitalWrite(RELAY_PIN, HIGH);
    delay(preferences.getLong("PumpRuntime"));
  }
}
