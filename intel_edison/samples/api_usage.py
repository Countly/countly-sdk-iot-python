from common.Countly import Countly

temperature = 30
light=50

countly = Countly("SERVER_URL", "APP_KEY", 5)
countly.event('Temperature', temperature)
countly.event("Light", light)
