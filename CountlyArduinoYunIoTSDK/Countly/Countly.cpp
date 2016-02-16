#include "Countly.h"
#include "HardwareSerial.h"
#include <Bridge.h>
#include <HttpClient.h>
#include "WString.h"
#include <Process.h>
#include <FileIO.h>

HttpClient client;
String mUrlString, mAppKey, mOsVersion;
String folderPath = "/mnt/sda1/";
const char *metricsTextFile = "metrics.txt";
const char *eventsTextFile = "events.txt";
const char *deviceId;
const int BUFSIZE = 32;
char buf[BUFSIZE];
EepromUtil eepromUtil;
const String appVersion="0.0.1";

Countly::Countly(String urlStr, String appKey) {
	mUrlString = urlStr;
	mAppKey = appKey;
	deviceId = getUuidFromEeprom();
	if (deviceId == "") {
		eepromUtil.eeprom_erase_all();
		String uuid = generateUuid();
		writeUuidToEeprom(uuid, BUFSIZE);
		deviceId = uuid.c_str();
	} else {
		deviceId = getUuidFromEeprom();
	}
}

void Countly::metrics() {
	Bridge.begin();
	FileSystem.begin();

	String osVersion = getOsVersion();

	String metricApiUrl = "http://" + mUrlString + "/i?begin_session=1&app_key=" +mAppKey + "&device_id=" + String(deviceId);

	String metricPostParam="metrics={\"_locale\":\"en\",\"_app_version\":\""+appVersion+"\",\"_device\":\"Yun\",\"_os\":\"Linux\",\"_os_version\":\""+osVersion+"\"}";

	String filePathStr = folderPath + metricsTextFile;
		const char *filePath = filePathStr.c_str();
		File dataFile = FileSystem.open(filePath, FILE_APPEND);
		if (dataFile) {
			dataFile.println(metricPostParam);
		}
		dataFile.close();
		delay(2000);

		File readDataFile = FileSystem.open(filePath, FILE_READ);

		String newText;

		boolean isSentAllUrls=true;
		while (readDataFile.available() > 0) {
			char c = readDataFile.read();
			if (c != '\n' && c != '\r') {
				newText += c;
			}
			if (c == '\n' || c == '\r') {
				if (newText != "" && newText != "\n" && newText != "\r"
						&& newText != " ") {
					Serial.print("CALL METRIC URL FROM QUEUE:");
					Serial.println(newText);

					String returnString;
					client.post(metricApiUrl, metricPostParam);
					while (client.available()) {
						char c = client.read();
						returnString += c;
					}
					if(returnString==""){
						isSentAllUrls=false;
					}else{
						Serial.println(returnString);
					}
				}
				newText="";
			}
		}
		if(isSentAllUrls){
			deleteFile(metricsTextFile);
		}
		readDataFile.close();
}

void Countly::event(String key, int sum) {
	Bridge.begin();
	FileSystem.begin();
	//metrics();

	String timeStamp = getTimeStamp();

	String eventApiUrl = "http://" + mUrlString + "/i?end_session=1&app_key=" +mAppKey;

	String eventPostParam= "device_id=" + String(deviceId)+"&events=[{\"count\":1,\"timestamp\":"+timeStamp+",\"sum\":"+sum+",\"key\":\""+key+"\"}]";

	String filePathStr = folderPath + eventsTextFile;
	const char *filePath = filePathStr.c_str();
	File dataFile = FileSystem.open(filePath, FILE_APPEND);
	if (dataFile) {
		dataFile.println(eventPostParam);
	}
	dataFile.close();
	delay(2000);

	File readDataFile = FileSystem.open(filePath, FILE_READ);

	String newText;

	boolean isSentAllUrls=true;
	while (readDataFile.available() > 0) {
		char c = readDataFile.read();
		if (c != '\n' && c != '\r') {
			newText += c;
		}
		if (c == '\n' || c == '\r') {
			if (newText != "" && newText != "\n" && newText != "\r"
					&& newText != " ") {
				Serial.print("CALL EVENTS URL FROM QUEUE:");
				Serial.println(newText);

				String returnString;
				client.post(eventApiUrl, eventPostParam);
				while (client.available()) {
					char c = client.read();
					returnString += c;
				}
				if(returnString==""){
					isSentAllUrls=false;
				}else{
					Serial.println(returnString);
				}
			}
			newText="";
		}
	}
	if(isSentAllUrls){
		deleteFile(eventsTextFile);
	}
	readDataFile.close();
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

String Countly::generateUuid() {
	int i;
	String uuidString;
	for (i = 0; i < 31; i++) {
		int topDigit = random(32);
		uuidString += "0123456789ABCDEFHIJKLMNOPRSTUVYZ"[topDigit];
	}

	return uuidString;
}

const char* Countly::getUuidFromEeprom() {
	eepromUtil.eeprom_read_string(100, buf, BUFSIZE);
	return buf;
}

bool Countly::writeUuidToEeprom(String uuid, int len) {
	eepromUtil.eeprom_erase_all();
	uuid.toCharArray(buf, BUFSIZE);
	char* bufferStr = (char*) buf;
	eepromUtil.eeprom_write_string(100, bufferStr);
	return true;
}

void Countly::writeTextToFile(const char *fileName, String text) {
	Bridge.begin();
	FileSystem.begin();
	String filePathStr = folderPath + fileName;
	const char *filePath = filePathStr.c_str();
	File dataFile = FileSystem.open(filePath, FILE_APPEND);
	if (dataFile) {
		dataFile.println(text);
		dataFile.close();
	}
}

void Countly::deleteFile(const char *fileName) {
	Bridge.begin();
	FileSystem.begin();
	String filePathStr = folderPath + fileName;
	const char *filePath = filePathStr.c_str();
	FileSystemClass fileSystemClass = FileSystemClass();
	fileSystemClass.remove(filePath);
}

void Countly::sendHttpGetFromQueue(const char *fileName, String apiUrl) {

	String filePathStr = folderPath + fileName;
	const char *filePath = filePathStr.c_str();
	File dataFile = FileSystem.open(filePath, FILE_READ);

	String newText;
	String returnString;

	while (dataFile.available() > 0) {
		char c = dataFile.read();
		if (c != '\n' && c != '\r') {
			newText += c;
		}
		if (c == '\n' || c == '\r') {
			if (newText != "" && newText != "\n" && newText != "\r"
					&& newText != " ") {
				Serial.print("CALL URL FROM QUEUE:");
				Serial.println(newText);

				HttpClient client;
				client.get(newText);
				while (client.available()) {
					char clientRead = client.read();
					returnString += clientRead;
				}
				Serial.println(returnString);
				newText = "";
			}
		}
	}
	dataFile.close();
}

String Countly::readAllTextFromFile(const char *fileName) {
	Bridge.begin();
	FileSystem.begin();
	String filePathStr = folderPath + fileName;
	const char *filePath = filePathStr.c_str();
	File dataFile = FileSystem.open(filePath, FILE_READ);
	String text;
	while (dataFile.available() > 0) {
		char c = dataFile.read();
		text += c;
	}
	dataFile.close();

	return text;
}

void Countly::sendHttpGet(String apiCallUrl) {

	String returnString;
	client.get(apiCallUrl);
	while (client.available()) {
		char c = client.read();
		returnString += c;
	}
	Serial.println(returnString);
}
