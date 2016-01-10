from raspberry_pi.python.sdk.Countly import Countly

temperature = 30
light=50

countly = Countly("try.count.ly", "4b35a3ec6edd429270806f53df6b1cde85cdbf48", "f46a1cfab72a11e5a9e734363bd15286", 20)
countly.event('Temperature', temperature)
countly.event("Light", light)