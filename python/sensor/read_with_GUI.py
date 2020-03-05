##############
## Script listens to sensor port and writes contents into a file
##############
## requires pySerial to be installed
import serial
from datetime import datetime
from time import time
import tkinter as tk


def calibrate():
    global look_up
    temp={}
    print("Calibratings")
    for data in original_look_up:
        look_up[data]=original_look_up[data]
    for data in original_look_up:
        f=datas[data]/original_look_up[data][0]
        for i in range(len(original_look_up[data])):
            look_up[data][i]=original_look_up[data][i]*f
    for data in look_up:
        temp=[]
        for i in range(len(look_up[data])-1):
            temp.append((look_up[data][i]+look_up[data][i+1])/2)
        look_up[data]=temp
    
    return


original_look_up={0:[296,317,355,381,390], 1:[357,395,444,464,480], 2:[366,406,432,446,456], 3:[299,355,380,405,430], 4:[350,420,442,461,476], 5:[380,413,455,470,481], 6:[317,356,384,390,480], 7:[650,810,820,845,880], 8:[330,367,387,418,431]}
look_up={}
for data in original_look_up:
    temp=[]
    for i in range(len(original_look_up[data])-1):
        temp.append((original_look_up[data][i]+original_look_up[data][i+1])/2)
    look_up[data]=temp

#port = '/dev/tty.usbmodemFA131'
port = 'COM3'
write_to_file_path = "output.txt"
baud_rate = 9600                    #In arduino, Serial.begin(baud_rate)

# current date and time
print("Time = ", datetime.now().time())
print("Timestamp =", datetime.timestamp(datetime.now()))

ser = serial.Serial(port, baud_rate)    # open sensor port
ser.timeout = 10                        # timeout 10 seconds
print(ser)             # check which port was really used
output_file = open(write_to_file_path, "a+")

#time_stamp=time()

w=tk.Tk()

text_var=[]
text_box=[]
scale=2
for i in range(9):
    text_var.append(tk.StringVar())
    text_var[-1].set("err")
    text_box.append(tk.Label(w, textvariable=text_var[-1], width=5*scale, height=2*scale, relief=tk.SUNKEN, font=("Courier", 30)))
    text_box[-1].grid(row=i//3, column=i%3)
tk.Button(w,text="Calibrate",command=calibrate).grid(row=3,column=1)
w.update()

while True:
    #print(look_up)
    datas={}
    while len(datas)<9:
        line = ser.readline()
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        try:
            datas[int(line.split(" ")[0])]=float(line.split(" ")[1][:-2])
        except:
            print("Error")
            continue
    #print(datas)
    #if time()-0.1>time_stamp:
    for data in look_up:
            #print(data, datas[data], end="\t")
        if datas[data]<look_up[data][0]:
                text="0 kg"
                #print("0 kg", end="\t")
        elif (datas[data]<=look_up[data][1] and datas[data]>look_up[data][0]):
                text="22 kg"
        elif (datas[data]<=look_up[data][2] and datas[data]>look_up[data][1]):
                text="45 kg"
                #print("45 kg", end="\t")
        elif (datas[data]<=look_up[data][3] and datas[data]>look_up[data][2]):
                text="65 kg"
                #print("65 kg", end="\t")
        elif (datas[data]<=4000 and datas[data]>look_up[data][3]):
                text="90 kg"
                #print("90 kg", end="\t")
        else:
                text="err"
                #print("err", end="\t")
##            if data==2 or data==5:
##                print("\n")
##            elif data==8:
##                print("\n----------\n")

            #text=datas[data]
        text_var[data].set(text)
        #time_stamp=time()
    w.update()
    #date = datetime.timestamp(datetime.now())
    #print(str(datetime.timestamp(datetime.now())) + "   " + line)
    #output_file.write(datetime.timestamp(datetime.now()) + line)
    output_file.write(line)

ser.close()             # close port
