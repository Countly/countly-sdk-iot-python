#include "WString.h"
#ifndef Countly_h
#define Countly_h

class Countly
{
  public:
	Countly (String urlStr, String appKey);
	String init(String urlStr, String appKey);
	String event(String key, int sum);
    String getTimeStamp();
    String getOsVersion();
};

#endif

