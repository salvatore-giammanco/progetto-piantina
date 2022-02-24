/*
 *  This sketch demonstrates how to save and fetch values into ESP32 memory.
 *  Here we use the Preferences library as EEPROM is deprecated
 */
#include <Preferences.h>
#include "WiFi.h"

const int MOISTURE_READING_PIN = 34;
const int RELAY_PIN = 23;

Preferences preferences;
const float AirValue = 470.0; //Value the moisture sensor gives when dry
const float WaterValue = 222.0; //Value the moisture sensor gives when submerged in water
const float MoistureTresh = 50.0; //Moisture percentage treshold
const long SamplingTime = 1000; //Time between two consecutive samplings of the soil mosture
const long ReadingsInt = 50; //Time between two consecutive moisture readings for a sampling
const short NumReading = 10; //Number of moisture readings for each sampling


//Function to map a value on a range
float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Function to print a value p on serial, along with a final value f which also generates a new line
template<typename T, typename F>
void print_on_serial(T p, F fin){
    Serial.print("Time: ");
    Serial.print(millis());
    Serial.print(", ");
    Serial.print("Moisture: ");
    Serial.print(p);
    Serial.println(fin);
}


//Function to sample the soil moisture
//It performs NumberReadings number of readings of the soil moisture, at times ReadingsInt
//The final moisture sample value is the average of all the readings
//The sample value is then converted into a percentage based on values AirValue and WaterValue
float read_soil_moisture_percent(){
    float soilMoistureValueAverage = 0;
    // Read the moisture NumberReadings times, separated by ReadingsInt milliseconds
    for(int i=0;i<NumReading;i++){
        soilMoistureValueAverage += ((float) analogRead(MOISTURE_READING_PIN)/NumReading); //Average of all the readings
        delay(ReadingsInt); //Pause between two readings
    }
    print_on_serial(soilMoistureValueAverage, ""); //Prints the absolute value
    float soilmoisturepercent = map_range(soilMoistureValueAverage, AirValue, WaterValue, 0.0, 100.0);
    return constrain(soilmoisturepercent, 0.0, 100.0); //Returns the percentage of moisture
}


void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
}


void loop() {
    float soilmoisturepercent = read_soil_moisture_percent();
    print_on_serial(soilmoisturepercent, '%');
    if(soilmoisturepercent < MoistureTresh){
        Serial.println("Starting pump");
        digitalWrite(RELAY_PIN, HIGH);
        delay(1000);
    }
    digitalWrite(RELAY_PIN, LOW);
    delay(SamplingTime);
}