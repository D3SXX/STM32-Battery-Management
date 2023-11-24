#lib
import json
import time
from iotticket.models import device
from iotticket.models import criteria
from iotticket.models import deviceattribute
from iotticket.models import datanodesvalue
from iotticket.client import Client
import minimalmodbus
import numpy as np

#main
# modbus_read fuction code 04 is added
def modbus_read_input_register(register_addr):
    # Define the serial port and communication parameters
    SERIAL_PORT = 'com6'  # Adjust this to your serial port
    SLAVE_ADDRESS = 5  # Adjust this to your specific Modbus device's address
    BAUD_RATE = 115200

    # Create a minimalmodbus instrument instance
    instrument = minimalmodbus.Instrument(SERIAL_PORT, SLAVE_ADDRESS)
    instrument.serial.baudrate = BAUD_RATE
    instrument.serial.timeout = 1  # Adjust the timeout as needed

    try:
        # Read a holding register (e.g., register address 0x0001)
        register_value = instrument.read_register(register_addr, functioncode=4)
        # Print the value
        print(f"Value at register {register_addr}: {register_value}")
    except Exception as e:
        print(f"Error: {str(e)}")
    finally:
        # Close the serial connection
        instrument.serial.close()
    
    return register_value


data = json.load(open("iot-ticket\Config.json"))
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
    # write datanode demo
    # some mock data here
    startRegisterAddress = 0
    for voltageData in voltage_data_collection:
        voltage = modbus_read_input_register(startRegisterAddress)/1000
        voltageData.set_value(voltage)
        voltageData.set_timestamp(int(round(time.time() * 1000)))
        startRegisterAddress = startRegisterAddress + 1
        print(f"{voltageData.name}: {voltageData.v} {voltageData.unit}")
        # if voltageData.name == "cell1_voltage":
        #     voltageData.set_value(modbus_read_voltage())
        #     voltageData.set_timestamp(int(round(time.time() * 1000)))
        # else:
        #     voltageData.set_value(voltage)
        #     voltageData.set_timestamp(int(round(time.time() * 1000)))
    # battery_current.set_value(current)
    # battery_current.set_timestamp(int(round(time.time() * 1000)))
    # battery_temp.set_value(temperature)
    # battery_temp.set_timestamp(int(round(time.time() * 1000)))

    current = modbus_read_input_register(startRegisterAddress)/1000
    battery_current.set_value(current)
    battery_current.set_timestamp(int(round(time.time() * 1000)))
    startRegisterAddress = startRegisterAddress + 1
    print(f"{battery_current.name}: {battery_current.v} {battery_current.unit}")

    temperature_int = modbus_read_input_register(startRegisterAddress)
    temperature_uint16 = np.uint16(temperature_int)
    temperature_int16 = np.int16(temperature_uint16)
    temperature_double = temperature_int16 / 10

    battery_temp.set_value(temperature_double)
    battery_temp.set_timestamp(int(round(time.time() * 1000)))
    print(f"{battery_temp.name}: {battery_temp.v} {battery_temp.unit}")
    # iotDataCollection = voltage_data_collection.copy()
    # iotDataCollection.append(battery_current)
    # iotDataCollection.append(battery_temp)
    
    print(iotClient.writedata(deviceId, *voltage_data_collection))
    print(iotClient.writedata(deviceId, battery_current))
    print(iotClient.writedata(deviceId, battery_temp))
    
    # voltage += 0.1
    # current += 1
    # temperature += 5

    # voltage = 2.7 if voltage >= 3.9 else voltage
    # current = 2.5 if current >= 40 else current
    # temperature = -20.5 if temperature >= 50 else temperature

    print("sleeping 5s...")
    time.sleep(5)
