#include <Countly.h>
#include <Bridge.h>
#include <math.h>

const int thresholdvalue=10;
float Rsensor;

Countly countly = Countly("try.count.ly","23122102d2e30c59bc86218982866f20871d1a34");

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int sensorValue = analogRead(0); 
  Rsensor=(float)(1023-sensorValue)*10/sensorValue;  
  Serial.println(countly.event("Light",Rsensor));
  delay(5000);
}