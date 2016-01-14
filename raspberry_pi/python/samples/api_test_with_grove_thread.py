import grovepi
import time

from raspberry_pi.python.sdk.Countly import Countly


temperature_sensor = 1
light_sensor = 0

countly = Countly("try.count.ly", "APP_KEY", 0)
while True:
    countly.event('Temperature', grovepi.temp(temperature_sensor,'1.1'))
    countly.event("Light", int(grovepi.analogRead(light_sensor)/10.24))
    time.sleep(10)