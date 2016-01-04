import httplib
import json
import threading
import time
import urllib

import system_info

urlStr=""
appKey=""
deviceId=""
periodSecond=0
connectionStr= ""

class Countly:

    def __init__(self, url_str, app_key,device_id, period_second):
        global urlStr,appKey,deviceId,periodSecond,connectionStr
        urlStr=url_str
        appKey=app_key
        deviceId=device_id
        periodSecond=period_second
        connectionStr = httplib.HTTPSConnection(urlStr,443)

    def init(self):
        self.init(1)

    def init(self,start_thrad):
        if start_thrad==1:
            threading.Timer(periodSecond, self.init).start()
        metrics = {
              "_os": system_info.system_info.platform(),
              "_os_version": system_info.system_info.os_version(),
              "_device": "Raspberry Pi 2",
              "_app_version": "0.0.1",
              "_locale": system_info.system_info.locale()
            }

        reqmetrics= urllib.quote(json.dumps(metrics))

        metrics_file = 'metrics.txt'
        with open(metrics_file, 'a+') as outfile:
            json.dump(reqmetrics, outfile)
            outfile.write('\n')

        with open(metrics_file) as inputfile:
            reqmetrics_from_file = inputfile.readlines()


        getrequest_init="https://"+urlStr + "/i?begin_session=1&app_key=" + appKey + "&device_id=" + deviceId + "&metrics=" + reqmetrics

        connectionStr.request("GET", getrequest_init)
        response = connectionStr.getresponse()
        print response.status, response.reason
        data = response.read()
        print data

    def event(self,key):
        self.init(0)
        threading.Timer(periodSecond, self.event, [key]).start()

        events = [{
            "count":1,
            "key":key,
            "timestamp": int(time.time())
        }]
        reqevents=urllib.quote(json.dumps(events))
        self.send_events(reqevents)



    def event(self, key, sum_int):
        self.init(0)
        threading.Timer(periodSecond, self.event, [key,sum_int]).start()

        events = [{
            "count":1,
            "key":key,
            "timestamp": int(time.time()),
            "sum": sum_int
        }]
        reqevents=urllib.quote(json.dumps(events))
        self.send_events(reqevents)

    def send_events(self,reqevents):
        getrequest_events="https://"+urlStr + "/i?end_session=1&session_duration=" + str(periodSecond) + "&app_key=" + appKey + "&device_id=" + deviceId + "&events="+reqevents
        connectionStr.request("GET", getrequest_events)
        response = connectionStr.getresponse()
        print response.status, response.reason
        data = response.read()
        print data
        connectionStr.close()
