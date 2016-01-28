from raspberry_pi.Raspberry_SDK.Countly import Countly

temperature = 30
light=50

countly = Countly("try.count.ly", "23122102d2e30c59bc86218982866f20871d1a34", 5)
countly.event('Temperature', temperature)
countly.event("Light", light)
