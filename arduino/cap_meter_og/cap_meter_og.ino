/*  RCTiming_capacitance_meter
 *   Paul Badger 2008
 *  Demonstrates use of RC time constants to measure the value of a capacitor
 *
 * Theory   A capcitor will charge, through a resistor, in one time constant, defined as T seconds where
 *    TC = R * C
 *
 *    TC = time constant period in seconds
 *    R = resistance in ohms
 *    C = capacitance in farads (1 microfarad (ufd) = .0000001 farad = 10^-6 farads )
 *
 *    The capacitor's voltage at one time constant is defined as 63.2% of the charging voltage.
 *
 *  Hardware setup:
 *  Test Capacitor between common point and ground (positive side of an electrolytic capacitor  to common)
 *  Test Resistor between chargePin and common point
 *  220 ohm resistor between dischargePin and common point
 *  Wire between common point and analogPin (A/D input)
 */

#define analogPin      0          // analog pin for measuring capacitor voltage
#define chargePin      13         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin   11         // pin to discharge the capacitor
#define resistorValue  3300000.0F   // change this to whatever resistor value you are using
                                  // F formatter tells compliler it's a floating point value
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

float startTime;
float elapsedTime;
float microFarads;                // floating point variable to preserve precision, make calculations
float nanoFarads;

////////////////////////////////////////////////////////
int const numReadings = 100;
float readings[numReadings];
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

void setup(){
  pinMode(chargePin, OUTPUT);     // set chargePin to output
  digitalWrite(chargePin, LOW);  

  Serial.begin(9600);             // initialize serial transmission for debugging

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // set prescale to 16
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
}

void loop(){
  digitalWrite(chargePin, HIGH);  // set chargePin HIGH and capacitor charging
  startTime = micros();

  while(analogRead(analogPin) < 648){       // 647 is 63.2% of 1023, which corresponds to full-scale voltage
  }

  elapsedTime= micros() - startTime;
 // convert microseconds to seconds ( 10^-6 ) and Farads to picoFarads ( 10^12 ),  net 10^6 (1000000)  
  microFarads = ((float)elapsedTime / resistorValue) * 1000000.0;


  // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = microFarads;
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

    Serial.println((float)average,5);
    
//  Serial.print(elapsedTime);       // print the value to serial port
//  Serial.print(" mS    ");         // print units and carriage return


//  if (microFarads > 1){
//    Serial.println((float)microFarads,5);       // print the value to serial port
//    Serial.println(" picoFarads");         // print units and carriage return
//  }
//  else
//  {
//    // if value is smaller than one microFarad, convert to nanoFarads (10^-9 Farad).
//    // This is  a workaround because Serial.print will not print floats
//
//    nanoFarads = microFarads * 1000.0;      // multiply by 1000 to convert to nanoFarads (10^-9 Farads)
//    Serial.print((float)nanoFarads);         // print the value to serial port
//    Serial.println(" nanoFarads");          // print units and carriage return
//  }
  

  /* dicharge the capacitor  */
  digitalWrite(chargePin, LOW);             // set charge pin to  LOW
  pinMode(dischargePin, OUTPUT);            // set discharge pin to output
  digitalWrite(dischargePin, LOW);          // set discharge pin LOW
  while(analogRead(analogPin) > 0){         // wait until capacitor is completely discharged
  }

  pinMode(dischargePin, INPUT);            // set discharge pin back to input

//  delay(500);
}
