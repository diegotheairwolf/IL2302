
#include <math.h>
//variable declaration
// pin 5 - pwm output
// pin 2 - detect flank
// pin A0 - measure capacitor voltage


const byte interruptPin2 = 2;
const byte interruptPin3 = 3;
volatile byte state = CHANGE;

// 1ms period - 500us half period
#define t 0.0005
#define v_max 5   // 5VDC
#define res 1000  //defined at 10M
#define multiplier 1000000000000.0

float sensor;
float sum;
float avg;
int new_value;
int last_value;
float cap; //calculated value of the capacitance
unsigned long t_run;

float volatile Vi = 0.0;
float volatile Vc = 0.0;
volatile int high = 0;
volatile int low = 0;

////////////////////////////////////////////////////////
int const numReadings = 300;
float readings[numReadings];
int readIndex = 0;                // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average
int count = 0;

void setup() {
  pinMode(interruptPin2, INPUT_PULLUP);  // to detect high flank
  attachInterrupt(digitalPinToInterrupt(interruptPin2), detectHigh, HIGH);
  pinMode(interruptPin3, INPUT_PULLUP);  // to detect low flank
  attachInterrupt(digitalPinToInterrupt(interruptPin3), detectLow, LOW);
  Serial.begin(9600);
  analogWrite(5,127); // to output square signal
  Serial.println("Hello World \n");
  //sensor_value = 3.5;

  // initialize moving average array
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}


void loop() {

  if (high == 1) {
    Vc = ((analogRead(0)*5.0)/1024.0);
    high = 0;
  }
  
  if (low == 1){
    // 5VDC and 10bit ADC used
    Vi = ((analogRead(0)*5.0)/1024.0);
    low = 0;
  }
  
  Serial.println(Vi);
    
    // perform calculation
    // multiply t to pass it to pico Farads
    //cap = (multiplier)*((-t)/(res*log((Vc-v_max)/(Vi-v_max))));


    // subtract the last reading:
    //total = total - readings[readIndex];
    // read from the sensor:
    //readings[readIndex] = cap;
    // add the reading to the total:
    //total = total + readings[readIndex];
    // advance to the next position in the array:
    //readIndex = readIndex + 1;
  
    // if we're at the end of the array...
    //if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      //readIndex = 0;
    //}

    // calculate the average:
    //average = total / numReadings;
    
  //else{
  //  last_value = new_value;
  //}

  // send it to the computer as ASCII digits
  //Serial.println(average);
  //delay(1);        // delay in between reads for stability
  count++;

  //if (count == 300){Serial.end();}
      
}


void detectHigh() {
  high = 1;
}

void detectLow(){
  low = 1;
}
