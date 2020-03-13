##############
## Script listens to sensor port and writes contents into a file
##############
## requires pySerial to be installed
import serial
from datetime import datetime
from time import time
import tkinter as tk

class Data_Handling():
    def __init__(self, GUI=False, output_file_path=False,printing_type=False):
        """ Data handling of the values
            Param:
                GUI (default=False): GUI active or not
                output_file_path   : Output file active (value is path) or not (False)
                printing_type      : Which data to printing in file (0-8 indicates which tile, -1 all of them)
        """
        self.original_look_up={0:[296,340,355,363,390], 1:[343,380,405,422,428], 2:[350,378,410,415,420], 3:[329,372,394,413,430], 4:[352,394,410,420,446], 5:[353,389,425,430,440], 6:[317,360,381,380,407], 7:[770,930,950,1000,1025], 8:[293,333,362,377,400]}
        self.look_up={}
        self.GUI=GUI                                        
        self.printing_type=printing_type
        self.data_printing_active=False
        self.output_file=output_file_path
        self.init_look_up()

        self.register_button=False
        
        f=open(self.output_file, "w")
        f.write("Time:"+str(datetime.now().time())+"\n")
        f.close()

        self.iteration_count=0

 
    def init_look_up(self):
        for data in self.original_look_up:
            temp=[]
            for i in range(len(self.original_look_up[data])-1):
                temp.append((self.original_look_up[data][i]+self.original_look_up[data][i+1])/2)
            self.look_up[data]=temp

    def calibrate(self):
        temp={}
        print("Calibratings")
        for data in self.original_look_up:
            self.look_up[data]=self.original_look_up[data]
        for data in self.original_look_up:
            f=datas[data]/self.original_look_up[data][0]
            for i in range(len(self.original_look_up[data])):
                self.look_up[data][i]=self.original_look_up[data][i]*f
        for data in self.look_up:
            temp=[]
            for i in range(len(self.look_up[data])-1):
                temp.append((self.look_up[data][i]+self.look_up[data][i+1])/2)
            self.look_up[data]=temp
        return

    def button_toggle_data_printing(self):
        if not self.data_printing_active:
            self.toggle_data_printing()
        else:
            print(self.iteration_count)
    def toggle_data_printing(self):
        if self.data_printing_active:
            self.data_printing_active=False
            if self.GUI:
                self.register_button.configure(bg='green')
            f=open(DH.output_file, "a+")
            f.write("\n")
            f.close()
            DH.iteration_count=0
        else:
            self.data_printing_active=True
            if self.GUI:
                self.register_button.configure(bg='red')
            f=open(DH.output_file, "a+")
            f.write("\n")
            f.close()


DH=Data_Handling(GUI=True, output_file_path="..\..\output.txt", printing_type=0)
NUMBER_OF_DATA=10000

#port = '/dev/tty.usbmodemFA131'
port = 'COM3'
baud_rate = 9600                    #In arduino, Serial.begin(baud_rate)

# current date and time
print("Time = ", datetime.now().time())
print("Timestamp =", datetime.timestamp(datetime.now()))

ser = serial.Serial(port, baud_rate)    # open sensor port
ser.timeout = 10                        # timeout 10 seconds
print(ser)             # check which port was really used
#time_stamp=time()

if DH.GUI:
    w=tk.Tk()
    text_var=[]
    text_box=[]
    scale=2
    for i in range(9):
        text_var.append(tk.StringVar())
        text_var[-1].set("err")
        text_box.append(tk.Label(w, textvariable=text_var[-1], width=5*scale, height=2*scale, relief=tk.SUNKEN, font=("Courier", 30)))
        text_box[-1].grid(row=i//3, column=i%3)
    tk.Button(w,text="Calibrate",command=DH.calibrate).grid(row=3,column=1)
    if DH.output_file:
        DH.register_button=tk.Button(w,text="Register data",command=DH.button_toggle_data_printing, bg='green')
        DH.register_button.grid(row=3,column=2)
    w.update()

while True:
    datas={}
    while len(datas)<9:
        line = ser.readline()
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        try:
            datas[int(line.split(" ")[0])]=float(line.split(" ")[1][:-2])
        except:
            print("Error")
            continue
    for data in DH.look_up:
        if datas[data]<DH.look_up[data][0]:
            text="0 kg"
        elif (datas[data]<=DH.look_up[data][1] and datas[data]>DH.look_up[data][0]):
            text="22 kg"
        elif (datas[data]<=DH.look_up[data][2] and datas[data]>DH.look_up[data][1]):
            text="45 kg"
        elif (datas[data]<=DH.look_up[data][3] and datas[data]>DH.look_up[data][2]):
            text="65 kg"
        elif (datas[data]<=4000 and datas[data]>DH.look_up[data][3]):
            text="90 kg"
        else:
            text="err"

        if DH.GUI:
            #text=datas[data]
            text_var[data].set(text)
            
        else:
            print(text,end=" ")
            if data==2 or data==5:
                print("\n")
            elif data==8:
                print("\n----------\n")         
    if DH.GUI:
        w.update()
    #date = datetime.timestamp(datetime.now())
    #print(str(datetime.timestamp(datetime.now())) + "   " + line)
    #output_file.write(datetime.timestamp(datetime.now()) + line)
    if DH.output_file and DH.data_printing_active:
        #DH.output_file.write(line)
        f=open(DH.output_file, "a+")
        if DH.printing_type==-1:
            f.write(str(datas)+"\t")
        else:
            f.write(str(datas[DH.printing_type])+"\t")
        f.close()
        DH.iteration_count+=1
        if DH.iteration_count>=NUMBER_OF_DATA:
            DH.toggle_data_printing()
        
            

ser.close()             # close port
