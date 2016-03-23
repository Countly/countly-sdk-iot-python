import sys
import os.path

sys.path.append(os.path.join(os.path.dirname(__file__), '../..'))

import grovepi

from common.Countly import Countly

temperature_sensor = 1
light_sensor = 0

countly = Countly("SERVER_URL", "APP_KEY", 5)
countly.event('Temperature', grovepi.temp(temperature_sensor,'1.1'))
countly.event("Light", int(grovepi.analogRead(light_sensor)/10.24))
