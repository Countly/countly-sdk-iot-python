#include <Countly.h>
#include <Bridge.h>

Countly countly = Countly("try.count.ly","23122102d2e30c59bc86218982866f20871d1a34");

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(countly.event("Temperature",25));
  delay(2000);
}