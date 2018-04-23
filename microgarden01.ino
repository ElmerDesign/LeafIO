#include <dht.h>
#include "Nextion.h"

dht DHT;
#define DHT11_PIN 4 // DHT11 connected to digital pin 4

//declare variables:

int h2o = A0; //(water level sensor)
int growlight = 7;
int pump = 8;
int fan = 9;

int growlightStatus = false; //set initial output states
int pumpStatus = false; //as above
int fanStatus = false; //as above

//declare all Nextion objects
NexText tempC = NexText(2, 6, "tempC"); //page ID then component ID on Nextion editor
NexText RH = NexText(2, 7, "RH"); //as above
NexProgressBar jWaterLevel = NexProgressBar(2, 2,"jWaterLevel"); //as above
NexButton bGrowlight = NexButton(3, 7, "bGrowlight");
NexButton bPump = NexButton(3, 5, "bPump");
NexButton bFan = NexButton(3, 6, "bFan");
NexButton bTakereadings = NexButton(2, 8, "bTakereadings");

//all touch components that trigger Arduino events:

NexTouch *nex_listen_list[] = {

    &bGrowlight,
    &bPump,
    &bFan,
    &bTakereadings,
    NULL
};

// 'PopCallback', callback functions triggered by touch events:

void bGrowlightPopCallback(void *ptr) {
  digitalRead(growlight); //reads current pin status
  growlightStatus = !growlightStatus; //inverts status
  digitalWrite(growlight, growlightStatus); //writes status to pin
  }

void bPumpPopCallback(void *ptr) {
  digitalRead(pump);
  pumpStatus = !pumpStatus;
  digitalWrite(pump, pumpStatus);
}
void bFanPopCallback(void *ptr) {
  digitalWrite(fan, HIGH);
  fanStatus = !fanStatus;
  digitalWrite(fan, fanStatus);
}

void bTakereadingsPopCallback(void *ptr) {

  char waterReading = analogRead(h2o) / 10.25; //take water level reading
  jWaterLevel.setValue(waterReading); //send water level to Nextion

  float chk = DHT.read11(DHT11_PIN); //takes DHT11 readings
  float  humidityReading = DHT.humidity;
  unsigned int temperatureReading = DHT.temperature;
  
  static char t[4]; //preparing to convert float to string for temp/humidity readings
  char h[6]; //as above

  dtostrf(humidityReading, 4, 1, h); //converts humidity reading from float to string
  utoa(temperatureReading, t, 10); //converts temperature reading from unsigned integer to string
  tempC.setText(t); //send temperature to Nextion
  RH.setText(h); //send humidity to Nextion
  }

void setup() {
  
  nexInit(); //initialise serial (refers to Nextion library)

  pinMode(h2o, INPUT);
  pinMode(growlight, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);

  bGrowlight.attachPop(bGrowlightPopCallback, &bGrowlight); //attach the popcallback snippet to touchscreen button
  bPump.attachPop(bPumpPopCallback, &bPump); //as above
  bFan.attachPop(bFanPopCallback, &bFan); //as above
  bTakereadings.attachPop(bTakereadingsPopCallback, &bTakereadings); //as above 
}


void loop() {

nexLoop(nex_listen_list); //listen for serial commands from nexion
}

