// DESCRIPTION
//   Prototype garage temperature logger
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


DHT sensors[NUM_SENSORS];
float humidity[NUM_SENSORS];
float temperature[NUM_SENSORS];
String record;


void setup() {
	Serial.begin(115200);
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


String render_as_json(float humidity[], float temperature[]) {
	String json = "{ ";
	json += "\"message\": \"Sensor readings\"";
	json += ", \"timestamp\": \""; json += millis(); json += "\"";

	for (int i = 0; i < NUM_SENSORS; i++) {
		if (sensor_data_is_bad(humidity[i], temperature[i])) continue;

		json += ", \"humidity_percent_"; json += i; json += "\": ";
		json += "\""; json += float_to_string(humidity[i]); json += "\"";
		json += ", \"temperature_celsius_"; json += i; json += "\": ";
		json += "\""; json += float_to_string(temperature[i]); json += "\"";
	}

	return json += " }";
}


void loop() {
	delay(2000);

	for (int i = 0; i < NUM_SENSORS; i++) {
		humidity[i] = sensors[i].readHumidity();
		temperature[i] = sensors[i].readTemperature();
	}

	record = render_as_json(humidity, temperature);

	Serial.println(record);
}




