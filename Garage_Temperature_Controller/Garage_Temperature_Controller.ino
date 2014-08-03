// DESCRIPTION
//   Prototype garage temperature logger, emits JSON records
// HARDWARE
//   3 x AM2301 temperature & humidity sensors
//   3 x 10k pull-ups
// AUTHOR
//   Craig J Perry, <craigp84@gmail.com>
//


#include "DHT.h"


// Configuration
#define NUM_SENSORS 3
#define SENSOR_TYPE AM2301
#define FIRST_SENSOR_PIN 2
#define SERIAL_SPEED 115200
#define DHT_POLL_DELAY 2000


DHT sensors[NUM_SENSORS];


void setup() {
	Serial.begin(SERIAL_SPEED);
	while(!Serial);  // Wait for serial, required at least on Leonardo devices

	for (int i = 0; i < NUM_SENSORS; i++) {
		sensors[i].begin(FIRST_SENSOR_PIN + i, SENSOR_TYPE);
	}
}


boolean sensor_data_is_bad(float humidity, float celsius) {
	return isnan(humidity) || isnan(celsius);
}


String float_to_string(float in) {
	char buffer[10];
	dtostrf(in, 2, 2, buffer);
	return String(buffer);
}


void loop() {
	delay(DHT_POLL_DELAY);

	Serial.print(F("{ \"message\": \"Sensor readings\", \"timestamp\": \"")); Serial.print(millis()); Serial.print(F("\""));

	for (int i = 0; i < NUM_SENSORS; i++) {

		float humidity = sensors[i].readHumidity();
		float temperature = sensors[i].readTemperature();
		
		if(sensor_data_is_bad(humidity, temperature)) {
			continue;
		}

		Serial.print(F(", \"humidity_percent_")); Serial.print(i); Serial.print(F("\": \"")); Serial.print(float_to_string(humidity)); Serial.print(F("\""));
		Serial.print(F(", \"temperature_celsius_")); Serial.print(i); Serial.print(F("\": \"")); Serial.print(float_to_string(temperature)); Serial.print(F("\""));
	}

	Serial.println(F(" }"));
}

