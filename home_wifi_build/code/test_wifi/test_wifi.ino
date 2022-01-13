/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"

#define WIFI_SSID "abc"
#define WIFI_PASS "def"
#define WIFI_TIMEOUT 20000

bool wifi_present(int n){
    for(int i=0; i<n; i++){
        if(WiFi.SSID(i) == WIFI_SSID) {
            return true;
        }
    }
    return false;
}

void connect_to_wifi(){
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        exit(1);
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");

    Serial.print("Wifi network ");
    Serial.print(WIFI_SSID);
    if(!wifi_present(n)){
        Serial.println(" not found!");
        exit(1);
    }
    else{
        Serial.println(" found!");
    }

    Serial.println("connecting to wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long startAttemptTime = millis();

    while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT) {
        Serial.println(".");
        delay(1000);
    }
    Serial.println();
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("Failed connection!");
        delay(1000);
        exit(1);
    }
    else{
        Serial.print("Connected to WiFi: ");
        Serial.println(WiFi.SSID(0));
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        delay(1000);
    }
}

void setup()
{
    Serial.begin(115200);
    // // Set WiFi to station mode and disconnect from an AP if it was previously connected
    // WiFi.mode(WIFI_STA);
    // WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
    connect_to_wifi();
}

void loop()
{}
