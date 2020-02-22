
#include <math.h>
//variable declaration
// pin 5 - pwm output
// pin 2 - detect flank
// pin A0 - measure capacitor voltage


// 1ms period - 500us half period
#define t 0.0005
#define v_max 5
#define res 6800000 //defined at 10M

float sensor;
float sum;
float avg;
int new_value;
int last_value;
float cap; //calculated value of the capacitance
unsigned long t_run;

////////////////////////////////////////////////////////
int const numReadings = 300;
float readings[numReadings];
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average
int count = 0;

void setup() {
  pinMode(2,INPUT); // to detect high flank
  Serial.begin(9600);
  analogWrite(5,127); // to output square signal
  Serial.print("Hello World \n");
  //sensor_value = 3.5;

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}




void loop() {
  new_value = digitalRead(2);
  
  if (new_value =! last_value)
  {

    if (new_value == 1)
    {

      delayMicroseconds(450); //delay of 500us, in order to read on half period

      
      // subtract the last reading:
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = ((analogRead(0)*5.0)/1024.0);
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;
    
      // if we're at the end of the array...
      if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }

        // calculate the average:
      average = total / numReadings;


      cap = (-t*1000000000000)/(res*log(1-(average/v_max)));

      // send it to the computer as ASCII digits
      Serial.println(cap);
      //delay(1);        // delay in between reads for stability
      count++;
      
     }
    else
    {
    last_value = new_value; 
    }
    
  }



  
  
  //if (count == 300){Serial.end();}   

}
