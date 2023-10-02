import minimalmodbus

# Define the serial port and communication parameters
SERIAL_PORT = 'com6'  # Adjust this to your serial port
SLAVE_ADDRESS = 5  # Adjust this to your specific Modbus device's address
BAUD_RATE = 9600

# Create a minimalmodbus instrument instance
instrument = minimalmodbus.Instrument(SERIAL_PORT, SLAVE_ADDRESS)
instrument.serial.baudrate = BAUD_RATE
instrument.serial.timeout = 1  # Adjust the timeout as needed

try:
    # Read a holding register (e.g., register address 0x0001)
    register_value = instrument.read_register(0x0005, functioncode=4)

    # Print the value
    print(f"Value at register 0x0001: {register_value}")
except Exception as e:
    print(f"Error: {str(e)}")
finally:
    # Close the serial connection
    instrument.serial.close()
