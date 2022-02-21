#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

Preferences preferences;
const char *SSID = "abc";
const char *PWD = "123";
float DefaultMoistureTresh = 50.0; //Moisture percentage treshold
// Web server running on port 80
WebServer server(80);
StaticJsonDocument<250> jsonDocument;
char buffer[250];

void create_json(char *tag, float value, char *unit) {
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);  
}

void getTreshold() {
  Serial.println("Get moisture");
  create_json("moisture", preferences.getFloat("MoistureTresh"), "%");
  server.send(200, "application/json", buffer);
}

void changeTreshold() {
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
  }
  Serial.println("Set moisture treshold");
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  // Get RGB components
  Serial.print("Setting moisture treshold to");
  Serial.println((float)jsonDocument["value"]);
  preferences.putFloat("MoistureTresh", jsonDocument["value"]);
  Serial.print("Moisture treshold set to ");
  Serial.println((float)preferences.getFloat("MoistureTresh"));
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void resetTreshold() {
  Serial.println("Reset moisture treshold to its default value");
  preferences.putFloat("MoistureTresh", DefaultMoistureTresh);
  Serial.print("Reset moisture treshold to ");
  Serial.println(DefaultMoistureTresh);
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void setup_routing() {
    //Sets a server routing so that each endpoint is assigned to an handler
    server.on("/treshold", getTreshold);
    server.on("/treshold/set", HTTP_POST, changeTreshold);
    server.on("/treshold/reset", HTTP_POST, resetTreshold);
    // start server
    server.begin();
}

void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.println(SSID);
    
    WiFi.begin(SSID, PWD);
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        // we can even make the ESP32 to sleep
    }
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    preferences.begin("constants", false);
    /*
    * If the script has been run before, there's already a value of moisture
    * Otherwise set the fixed one
    */
    if(preferences.isKey("MoistureTresh")){
        Serial.println("MoistureTresh key found");
        Serial.println(preferences.getFloat("MoistureTresh"));
    }
    else{
        Serial.println("MoistureTresh key not found");
        preferences.clear();
        preferences.putFloat("MoistureTresh", DefaultMoistureTresh);
    }
    connectToWiFi();
    setup_routing();
}

void loop() {
    server.handleClient();
}
