#include "thingProperties.h"

int LED = 23;
int us_to_ms = 1000000;
int seconds = 5;

void setup() {
  pinMode(LED, OUTPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
  Serial.println("Set timer");
  esp_sleep_enable_timer_wakeup(seconds * us_to_ms);
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  // Your code here
  random_value = random(0, 500);
  delay(500);
  ArduinoCloud.update();
  Serial.println("Going to deep sleep");
  esp_deep_sleep_start();
}

void loop() {}


void onLedSwitchChange() {
  // Do something
  if (led_switch) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}