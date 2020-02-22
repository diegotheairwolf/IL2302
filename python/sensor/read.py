##############
## Script listens to sensor port and writes contents into a file
##############
## requires pySerial to be installed
import serial
from datetime import datetime

port = '/dev/tty.usbmodemFA131'
write_to_file_path = "output.txt"
baud_rate = 9600                    #In arduino, Serial.begin(baud_rate)

# current date and time
print("Time = ", datetime.now().time())
print("Timestamp =", datetime.timestamp(datetime.now()))

ser = serial.Serial(port, baud_rate)    # open sensor port
ser.timeout = 10                        # timeout 10 seconds
print(ser)             # check which port was really used
output_file = open(write_to_file_path, "a+")
while True:
    line = ser.readline()
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    print(line)
    #date = datetime.timestamp(datetime.now())
    #print(str(datetime.timestamp(datetime.now())) + "   " + line)
    #output_file.write(datetime.timestamp(datetime.now()) + line)
    output_file.write(line)

ser.close()             # close port