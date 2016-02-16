#include "WString.h"
#include <EepromUtil.h>
#include <stdbool.h>
#include <stdint.h>
#include <WString.h>
#include <FileIO.h>

#ifndef Countly_h
#define Countly_h

class Countly
{
  public:
	Countly (String urlStr, String appKey);
	void metrics();
	void event(String key, int sum);
    String getTimeStamp();
    String getOsVersion();
    String generateUuid();
    const char* getUuidFromEeprom();
    bool writeUuidToEeprom(String uuid, int len);
    void writeTextToFile(const char *fileName, String text);
    void sendHttpGetFromQueue(const char *fileName, String apiUrl);
    String readAllTextFromFile(const char *fileName);
    void deleteFile(const char *fileName);
    void sendHttpGet(String apiCallUrl);
};

#endif

