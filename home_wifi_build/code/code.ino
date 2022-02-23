#include "memory.h"
#include "server.h"
#include "constants.h"


//Function to map a value on a range
float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//Function to print a value p on serial, along with a final value f which also generates a new line
template<typename T, typename F>
void print_on_serial(T p, F fin){
  Serial.print(millis());
  Serial.print(": ");
  Serial.print(p);
  Serial.println(fin);
}


//Function to sample the soil moisture
//It performs NumberReadings number of readings of the soil moisture, at times ReadingsIntervalMS
//The final moisture sample value is the average of all the readings
//The sample value is then converted into a percentage based on values AirValue and WaterValue
float read_soil_moisture_percent(){
  float soilMoistureValueAverage = 0;
  // Read the moisture NumberReadings times, separated by ReadingsIntervalMS milliseconds
  for(int i=0;i<DefaultNumReading;i++){
    soilMoistureValueAverage += ((float) analogRead(MOISTURE_READING_PIN)/DefaultNumReading); //Average of all the readings
    delay(DefaultReadingsInt); //Pause between two readings
  }
  print_on_serial('\r', soilMoistureValueAverage); //Prints the absolute value
  float soilmoisturepercent = map_range(soilMoistureValueAverage, DefaultAirValue, DefaultWaterValue, 0.0, 100.0);
  return constrain(soilmoisturepercent, 0.0, 100.0); //Returns the percentage of moisture
}


void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
  }
  preferences.begin("constants", false);
  /*
  * If the script has been run before, there's already a value of moisture
  * Otherwise set the fixed one
  */
  Serial.println("Reading AirValue");
  read_var(DefaultAirValue, "AirValue");
  connectToWiFi();
  setup_routing();
}


void loop() {
  digitalWrite(RELAY_PIN, LOW);
  delay(DefaultSamplingTime);

  float soilmoisturepercent = read_soil_moisture_percent();
  print_on_serial(soilmoisturepercent, '%');

  if(soilmoisturepercent < DefaultMoistureTresh){
    digitalWrite(RELAY_PIN, HIGH);
    delay(DefaultPumpRuntime);
  }
}
