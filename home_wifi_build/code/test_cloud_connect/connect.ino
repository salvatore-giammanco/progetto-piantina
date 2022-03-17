#include "thingProperties.h"

bool synced = false;
int LED = 2;
int us_to_ms = 1000000;
int seconds = 10;
RTC_DATA_ATTR int boot = 0;

void setup() {
  pinMode(LED, OUTPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial.println("Started "+String(boot++)+" times");
  Serial.println("Setting timer wakeup");
  esp_sleep_enable_timer_wakeup(seconds * us_to_ms);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
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
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    ArduinoCloud.update();
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connecting to Arduino IoT and syncing variables");
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, updateOnSync);
  while(!synced){
    ArduinoCloud.update();
    Serial.print(".");
    delay(500);
  }
  delay(1000);
  // Your code here
  Serial.println("Updating");
  Serial.println("Connected: "+String(ArduinoCloud.connected()));
  Serial.println("Attached: "+String(!ArduinoCloud.deviceNotAttached()));
  Serial.println("Sync: "+String(synced));
  delay(500);
  random_value = random(0, 500);
  delay(500);
  ArduinoCloud.update();
  Serial.println("random: "+String(random_value));
  Serial.println("led: "+String(led_switch));
  delay(5000);
  Serial.println("Going to sleep for "+String(seconds)+" seconds");
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

void updateOnSync(){
  /* add your custom code here */
  Serial.println("Thing Properties synchronised");
  synced = true;
}