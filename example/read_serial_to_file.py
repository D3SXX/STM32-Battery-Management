import serial
import argparse

class DataRecorder:
    def __init__(self,filename):
        if filename == "None":
            self.f = None
            return
        try:
            self.f = open(filename,"a")
        except:
            self.f = open(filename,"w")
    def write(self,text):
        if self.f == None:
            return
        self.f.write(text)
    def close(self):
        if self.f == None:
            return
        self.f.close()

def main(args):
    try:
        ser = serial.Serial(args.port, args.baudrate)

        data_file = DataRecorder(args.output)

        while True:
            data = ser.readline()
            text = data.decode('utf-8')
            print(text, end="")
            data_file.write(text)

    except KeyboardInterrupt:
        print("Keyboard interrupt detected")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        ser.close()
        print("Serial port was closed.")
        data_file.close()
        print("The file was closed.")

if __name__ == '__main__':

    args = argparse.ArgumentParser()
    args.add_argument("-p", "--port",required=True, help="Define serial port (e.g. COM4 or /dev/ttyUSB0)")
    args.add_argument("-b", "--baudrate",type=int, default=9600, help="Define baudrate (default: 9600)")
    args.add_argument("-o","--output",default="data.txt", help='Output file name (use None to disable output) (default data.txt)')
    args = args.parse_args()
    main(args)