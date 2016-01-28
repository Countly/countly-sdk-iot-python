#include "Countly.h"
#include "HardwareSerial.h"
#include <Bridge.h>
#include <HttpClient.h>
#include "WString.h"
#include <Process.h>

HttpClient client;
String mUrlString, mAppKey, mOsVersion;

Countly::Countly(String urlStr, String appKey) {
	mUrlString = urlStr;
	mAppKey = appKey;
}

String Countly::init(String urlStr, String appKey) {
	Bridge.begin();
	String returnString;
	String osVersion = getOsVersion();
	String apiUrl =
			"http://" + urlStr + "/i?begin_session=1&app_key=" + appKey
					+ "&device_id=device_yun&metrics=%7B%22_locale%22%3A%5B%22en_US%22%2C%22UTF-8%22%5D%2C%22_app_version%22%3A%220.0.1%22%2C%22_device%22%3A%22ArduinoYun%22%2C%22_os%22%3A%22OpenWRT%22%2C%22_os_version%22%3A%22"+osVersion+"%22%7D";
	client.get(apiUrl);

	while (client.available()) {
		char c = client.read();
		returnString += c;
	}

	return returnString;
}

String Countly::event(String key, int sum) {
	Bridge.begin();
	init(mUrlString, mAppKey);
	String returnString;
	String timeStamp = getTimeStamp();
	String apiUrl =
			"http://" + mUrlString + "/i?end_session=1&app_key=" + mAppKey
					+ "&device_id=device_yun&events=%5B%7B%22count%22%3A1%2C%22timestamp%22%3A"
					+ timeStamp + "%2C%22sum%22%3A" + sum + "%2C%22key%22%3A%22"
					+ key + "%22%7D%5D";
	client.get(apiUrl);

	while (client.available()) {
		char c = client.read();
		returnString += c;
	}

	return returnString;
}

String Countly::getTimeStamp() {
	String result;
	Process time;
	time.begin("date");
	time.addParameter("+%s");
	time.run();

	while (time.available() > 0) {
		char c = time.read();
		if (c != '\n')
			result += c;
	}

	return result;
}

String Countly::getOsVersion() {
	String result;
	Process time;
	time.begin("uname");
	time.addParameter("-r");
	time.run();

	while (time.available() > 0) {
		char c = time.read();
		if (c != '\n')
			result += c;
	}

	return result;
}
