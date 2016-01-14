from raspberry_pi.python.sdk.Countly import Countly

temperature = 30
light=50

countly = Countly("try.count.ly", "APP_KEY", 5)
countly.event('Temperature', temperature)
countly.event("Light", light)