#!/usr/bin/env python

import urllib2, ssl, csv, json, serial

domoticz_proto = "https"
domoticz_host = "domos.zerros.lan"
sensors_list = "sensors.list"
TTY="arduino1"

com = serial.Serial('/dev/'+TTY, 115200, timeout=.1)

data = {}

def loadSensors ():
  with open(sensors_list, "rb") as f:
    reader = csv.reader(f, delimiter=";")
    for i, line in enumerate(reader):
      if "#" not in line[0]:
        index = line[0]
        del line[0]
        data[index] = {
          "temp_idx": line[0],
          "hum_idx": line[1]
        }
    return data

def sendToDomoticz (sensorType, idx, value):
  if sensorType == "HUM":
    url = domoticz_proto+'://'+domoticz_host+'/json.htm?type=command&param=udevice&idx='+idx+'&nvalue='+value+'&svalue=0'
  if sensorType == "TEMP":
    url = domoticz_proto+'://'+domoticz_host+'/json.htm?type=command&param=udevice&idx='+idx+'&nvalue=0&svalue='+value
 
  request = urllib2.Request(url)
  response = urllib2.urlopen(request, context=ssl._create_unverified_context())
  print sensorType + " - " + idx + " - " + value
  print "---------------------"
  print response.read()

sensors = loadSensors()

while 1:
  data = com.readline()

  if data:
    tmp = json.loads(data)
    print tmp
  
    if str(tmp['sensor']) in sensors:
      sendToDomoticz("TEMP", sensors.get(str(tmp["sensor"]))['temp_idx'], str(tmp["temp"]))
      sendToDomoticz("HUM", sensors.get(str(tmp["sensor"]))['hum_idx'], str(tmp["hum"]))
    else:
      print "ERROR:: device not found"
