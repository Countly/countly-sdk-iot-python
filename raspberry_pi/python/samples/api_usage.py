from raspberry_pi.python.sdk.Countly import Countly

temperature = 30
light=50

countly = Countly("try.count.ly", "APP_KEY", "DEVICE_ID", 10)
countly.event('Temperature', temperature)
countly.event("Ligth", light)