#define MOISTURE_READING_PIN A0
#define RELAY_PIN 8

const float AirValue = 470.0; //Value the moisture sensor gives when dry
const float WaterValue = 222.0; //Value the moisture sensor gives when submerged in water
const int MS_minute = 60000; //Milliseconds per minute
const long MS_hour = (long)MS_minute*60; //Milliseconds per hour
const long SamplingTimeMS = MS_hour; //Time between two consecutive samplings of the soil mosture
const long PumpRunningTimeMS = MS_minute; //Time the pump runs when activated
const short NumberReadings = 10; //Number of moisture readings for each sampling
const long ReadingsIntervalMS = MS_minute/2; //Time between two consecutive moisture readings for a sampling
const short MoisturePercTreshold = 50;

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
  for(int i=0;i<NumberReadings;i++){
    soilMoistureValueAverage += ((float) analogRead(MOISTURE_READING_PIN)/NumberReadings); //Average of all the readings
    delay(ReadingsIntervalMS); //Pause between two readings
  }
  print_on_serial('\r', soilMoistureValueAverage); //Prints the absolute value
  float soilmoisturepercent = map_range(soilMoistureValueAverage, AirValue, WaterValue, 0.0, 100.0);
  return constrain(soilmoisturepercent, 0.0, 100.0); //Returns the percentage of moisture
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  Serial.flush();
}

void loop() {
  digitalWrite(RELAY_PIN, LOW);
  delay(SamplingTimeMS);

  float soilmoisturepercent = read_soil_moisture_percent();
  print_on_serial(soilmoisturepercent, '%');

  if(soilmoisturepercent < MoisturePercTreshold){
    digitalWrite(RELAY_PIN, HIGH);
    delay(PumpRunningTimeMS);
  }
 }
