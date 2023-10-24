from pymodbus.client import ModbusSerialClient

# Define the serial port and baud rate for Modbus RTU communication
SERIAL_PORT = 'com6'  # Adjust this to your serial port
BAUD_RATE = 9600

# Create a ModbusSerialClient instance for RTU communication
client = ModbusSerialClient(method='rtu', port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=1)

# Open the connection to the Modbus RTU server (slave)
if client.connect():
    # Specify the slave ID (unit ID) of the Modbus RTU device you want to communicate with
    SLAVE_ID = 0x05  # Adjust this to your specific device's ID

    # Define the Modbus address of the holding register you want to read
    REGISTER_ADDRESS = 0x0002  # Adjust this to your specific register address

    # Try to read the holding register
    # result = client.read_holding_registers(REGISTER_ADDRESS, 1, slave=SLAVE_ID)
    result = client.read_input_registers(address= REGISTER_ADDRESS, count = 1, slave= SLAVE_ID)

    # Check if the read operation was successful
    if not result.isError():
        print(f"Value at register {REGISTER_ADDRESS}: {result.registers[0]/10000} volt")
    else:
        print(f"Failed to read the input register: {result}")
    
    # Close the Modbus RTU connection
    client.close()
else:
    print("Failed to connect to the Modbus RTU server")
