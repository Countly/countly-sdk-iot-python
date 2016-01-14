import httplib
import json
import locale
import platform
import urllib

import time

import threading
import uuid

urlStr=""
appKey=""
deviceId=""
periodSecond=0
connectionStr= ""
events_queue_file = "events.txt"
metrics_queue_file = "metrics.txt"
uuid_file = "uuid.txt"

class Countly:

    def __init__(self, url_str, app_key,period_second):
        global urlStr,appKey,deviceId,periodSecond,connectionStr
        urlStr=url_str
        appKey=app_key
        deviceId=self.get_device_id()
        periodSecond=period_second


    def init(self):
        self.init(1)

    def init(self, start_thread):
        if start_thread==1 and periodSecond>0:
            threading.Timer(periodSecond, self.init,[start_thread]).start()
        metrics = {
              "_os":str(platform.system()),
              "_os_version": str(platform.release().replace("l","")),
              "_device": platform.machine(),
              "_app_version": "0.0.1",
              "_locale": locale.getdefaultlocale()
            }

        reqmetrics= urllib.quote(json.dumps(metrics))
        self.write_file(metrics_queue_file, reqmetrics)
        queuemetrics=self.get_queue_text(metrics_queue_file)

        if len(queuemetrics) >2:
          for queue in queuemetrics:
            getrequest_init="https://"+urlStr + "/i?begin_session=1&app_key=" + appKey + "&device_id=" + deviceId + "&metrics=" + queue.strip()
            self.connection_response(getrequest_init, metrics_queue_file)

        else:
          getrequest_init="https://"+urlStr + "/i?begin_session=1&app_key=" + appKey + "&device_id=" + deviceId + "&metrics=" + queuemetrics[0].strip()
          self.connection_response(getrequest_init, metrics_queue_file)

    def event(self,key):
        self.init(0)
        if(periodSecond>0):
            threading.Timer(periodSecond, self.event,[key]).start()

        events = [{
            "count":1,
            "key":key,
            "timestamp": int(time.time())
        }]
        reqevents=urllib.quote(json.dumps(events))
        self.write_file(events_queue_file, reqevents)
        queueevent=self.get_queue_text(events_queue_file)
        if len(queueevent) >2:
             for queue in queueevent:
               getrequest_events="https://"+urlStr + "/i?end_session=1&session_duration=" + str(periodSecond) + "&app_key=" + appKey + "&device_id=" + deviceId + "&events="+queue.strip()
               self.connection_response(getrequest_events, events_queue_file)
        else:
            getrequest_events="https://"+urlStr + "/i?end_session=1&session_duration=" + str(periodSecond) + "&app_key=" + appKey + "&device_id=" + deviceId + "&events="+queueevent[0].strip()
            self.connection_response(getrequest_events, events_queue_file)


    def event(self, key, sum_int):
        self.init(0)
        if(periodSecond>0):
            threading.Timer(periodSecond, self.event,[key,sum_int]).start()

        events = [{
            "count":1,
            "key":key,
            "timestamp": int(time.time()),
            "sum": sum_int
        }]
        reqevents=urllib.quote(json.dumps(events))

        self.write_file(events_queue_file, reqevents)
        queueevent=self.get_queue_text(events_queue_file)
        if len(queueevent) >2:
            for queue in queueevent:
               getrequest_events="https://"+urlStr + "/i?end_session=1&session_duration=" + str(periodSecond) + "&app_key=" + appKey + "&device_id=" + deviceId + "&events="+queue.strip()
               self.connection_response(getrequest_events, events_queue_file)
        else:
            getrequest_events="https://"+urlStr + "/i?end_session=1&session_duration=" + str(periodSecond) + "&app_key=" + appKey + "&device_id=" + deviceId + "&events="+queueevent[0].strip()
            self.connection_response(getrequest_events, events_queue_file)


    def connection_response(self, getrequest_url, filename):
        try:
            connectionStr = httplib.HTTPSConnection(urlStr,443)
            connectionStr.request("GET", getrequest_url)
            response = connectionStr.getresponse()
            print response.status, response.reason
            data = response.read()
            print data
            connectionStr.close()
            self.delete_last_line_from_file(filename);
        except Exception, e:
           print str(e)


    def write_file(self, filename, text):
        with open(filename, 'a+') as outfile:
            outfile.write(text+'\n')
        outfile.close()

    def read_file(self,filename):
        with open(filename) as readfile:
           return readfile.readlines()

    def delete_last_line_from_file(self, filename):
        readFile = open(filename)
        lines = readFile.readlines()
        readFile.close()
        w = open(filename,'w')
        w.writelines([item for item in lines[:-1]])
        w.close()

    def get_device_id(self):
        device_id=""
        try:
            device_id=self.read_file(uuid_file)[0].strip()
            if(device_id):
                return device_id
            else:
                device_id = self.write_device_id(device_id)
        except Exception:
            device_id = self.write_device_id(device_id)
        return device_id

    def write_device_id(self, device_id):
        self.write_file(uuid_file, str(uuid.uuid1()).replace("-", ""))
        device_id = self.read_file(uuid_file)[0].strip()
        return device_id

    def get_queue_text(self, filename):
        queuetext=""
        try:
            queuetext=self.read_file(filename)
            if(queuetext):
                return queuetext
            else:
                queuetext=""
        except Exception:
            queuetext=""
        return queuetext