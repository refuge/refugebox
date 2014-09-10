#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// Sensors
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
// used the 3rd parameter to 30 because the Udoo use a higher frequency (84MHz)
DHT_Unified dht(DHTPIN, DHTTYPE, 30);

String payload;
float temperature;

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) {
  Serial.begin(9600);
  
  dht.begin();
  bmp.begin();
  
  // Blinking LED when a payload is sent
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) {
  /* Get a new sensor event */ 
  sensors_event_t event;
  payload = "";
 
  bmp.getEvent(&event);
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure) {
    payload += "P1:";
    payload += event.pressure;
    
    bmp.getTemperature(&temperature);
    payload +=";T1:";
    payload += temperature;
  }
  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    if (payload.length() != 0) {
      payload += ";";
    }
    
    payload += "T2:";
    payload += event.temperature;
  }
  
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    if (payload.length() != 0) {
      payload += ";";
    }
    
    payload += "H1:";
    payload += event.relative_humidity;
    
    // Debug lines
    //Serial.print("Humidity: ");
    //Serial.print(event.relative_humidity);
    //Serial.println("%");
  }
  
  if (payload.length() != 0) {
    Serial.println(payload);
      
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  }
  else {
    Serial.print("0005error");
  }
  
  // measure every 30s
  delay(30000);
}

void padding( int number, byte width ) {
  int currentMax = 10;
  for (byte i=1; i<width; i++){
    if (number < currentMax) {
      Serial.print("0");
    }
    currentMax *= 10;
  } 
  Serial.print(number);
}
