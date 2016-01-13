import grovepi
from raspberry_pi.python.sdk.Countly import Countly


temperature_sensor = 1
light_sensor = 0

temperature = grovepi.temp(temperature_sensor,'1.1')
light=int(grovepi.analogRead(light_sensor)/10.24)

countly = Countly("try.count.ly", "APP_KEY", 5)
countly.event('Temperature', temperature)
countly.event("Light", light)