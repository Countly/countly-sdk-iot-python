#include <Countly.h>
#include <Bridge.h>

Countly countly = Countly("SERVER_URL", "APP_KEY");

void setup() {
	Serial.begin(9600);
}

void loop() {
	countly.metrics();
	delay(3000);
	countly.event("Temperature", 25);
	delay(2000);
}
