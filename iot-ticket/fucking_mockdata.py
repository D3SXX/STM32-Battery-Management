#lib
import json
import sys
import time
import random
import serial
from iotticket.models import device
from iotticket.models import criteria
from iotticket.models import deviceattribute
from iotticket.models import datanodesvalue
from iotticket.client import Client

#main

data = json.load(open("config.json"))
username = data["username"]
password = data["password"]
deviceId = data["deviceId"]
baseurl = data["baseurl"]
iotClient = Client(baseurl, username, password)

voltage = 2.7
current = 2.5
temperature = -20.5

cell_1_voltage = datanodesvalue(name="cell1_voltage", dataType="double", unit="Volts")
cell_1_voltage.set_path("cell1_volt")
cell_2_voltage = datanodesvalue(name="cell2_voltage", path="/cell2_volt", dataType="double", unit="Volts")
cell_3_voltage = datanodesvalue(name="cell3_voltage", path="/cell3_volt", dataType="double", unit="Volts")
cell_4_voltage = datanodesvalue(name="cell4_voltage", path="/cell4_volt", dataType="double", unit="Volts")
battery_current = datanodesvalue(name="battery_current", path="/batt_current", dataType="double", unit="Amps")
battery_temp = datanodesvalue(name="battery_temperature", dataType="double", unit="C")
battery_temp.set_path("batt_temp")

voltage_data_collection = list()
voltage_data_collection.append(cell_1_voltage)
voltage_data_collection.append(cell_2_voltage)
voltage_data_collection.append(cell_3_voltage)
voltage_data_collection.append(cell_4_voltage)


while True:
    #write datanode demo
    for voltageData in voltage_data_collection:
        voltageData.set_value(voltage)
        voltageData.set_timestamp(int(round(time.time() * 1000)))
    battery_current.set_value(current)
    battery_current.set_timestamp(int(round(time.time() * 1000)))
    battery_temp.set_value(temperature)
    battery_temp.set_timestamp(int(round(time.time() * 1000)))

    iotDataCollection = voltage_data_collection
    iotDataCollection.append(battery_current)
    iotDataCollection.append(battery_temp)
    
    
    print(iotClient.writedata(deviceId, *iotDataCollection))
    
    voltage += 0.1
    current += 1
    temperature += 5

    voltage = 2.7 if voltage >= 3.9 else voltage
    current = 2.5 if current >= 40 else current
    temperature = -20.5 if temperature >= 50 else temperature

    print("sleeping 3s...")
    time.sleep(3)
