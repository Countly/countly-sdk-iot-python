#include "CountlySSL.h"

#include "HardwareSerial.h"
#include <Bridge.h>
#include <Process.h>

Process proc;

CountlySSL::CountlySSL() {

}

String CountlySSL::init(String urlStr, String appKey) {
	Bridge.begin();
	String returnString;
	proc.begin("curl");
	proc.addParameter("-k");
	String apiUrl = "https://" + urlStr + "/i?begin_session=1&app_key=" + appKey+"&device_id=device_yun";
	proc.addParameter(apiUrl);
	proc.run();
	while (!proc.available()) {
		delay(100);
	}

	bool print = true;

	while (proc.available() > 0) {
		char c = proc.read();
		returnString += c;
	}

	return returnString;
}
