const int PIN = 4;
float max_v = 0;
float min_v = 4095;

void setup(){
    Serial.begin(115200);
    while(!Serial){}
    pinMode(PIN, INPUT);
}

void loop(){
    float reading = ((float) analogRead(PIN)/4095.0)*3.3;
    if(reading > max_v) max_v = reading;
    if(reading < min_v) min_v = reading;
    Serial.println("Val: "+String(reading)+" V, Max: "+String(max_v)+" V, Min: "+String(min_v)+" V");
    delay(500);
}