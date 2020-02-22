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
 *  Test Resistor between chargePin13 and common point
 *  220 ohm resistor between dischargePin11 and common point
 *  Wire between common point and analogPin (A/D input)
 */

#define analogPin      0          // analog pin for measuring capacitor voltage
#define chargePin13      13         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin11   11         // pin to discharge the capacitor
#define chargePin12      12         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin10   10         // pin to discharge the capacitor
#define A              4          // pin to select multiplexer channel. LSB
#define B              3          // pin to select multiplexer channel.
#define C              2          // pin to select multiplexer channel. MSB
#define resistorValue  1000000.0F   // change this to whatever resistor value you are using
                                  // F formatter tells compliler it's a floating point value
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                // floating point variable to preserve precision, make calculations
float nanoFarads;

void setup(){
  pinMode(chargePin13, OUTPUT);     // set charegePin13 to output
  digitalWrite(chargePin13, LOW);
  pinMode(chargePin12, OUTPUT);     // set charegePin13 to output
  digitalWrite(chargePin12, LOW);   

  pinMode(A, OUTPUT);     // pin to select multiplexer channel. LSB
  digitalWrite(A, LOW);
  pinMode(B, OUTPUT);     // pin to select multiplexer channel.
  digitalWrite(B, LOW);
  pinMode(C, OUTPUT);     // pin to select multiplexer channel. MSB
  digitalWrite(C, LOW);
  
  Serial.begin(9600);             // initialize serial transmission for debugging

  // set prescale to 16
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
}

void loop(){
  digitalWrite(chargePin13, HIGH);  // set charegePin13 HIGH and capacitor charging
  digitalWrite(chargePin12, HIGH);  // set charegePin13 HIGH and capacitor charging
  startTime = millis();
  
//  startTime = millis();

  while(analogRead(analogPin) < 648){       // 647 is 63.2% of 1023, which corresponds to full-scale voltage
  }

  elapsedTime= millis() - startTime;
 // convert milliseconds to seconds ( 10^-3 ) and Farads to microFarads ( 10^6 ),  net 10^3 (1000)  
  microFarads = ((float)elapsedTime / resistorValue) * 1000.0;  
  Serial.print(elapsedTime);       // print the value to serial port
  Serial.print(" mS    ");         // print units and carriage return


//  if (microFarads > 1){
    Serial.print((float)microFarads,5);       // print the value to serial port
    Serial.println(" microFarads");         // print units and carriage return
//  }
//  else
//  {
    // if value is smaller than one microFarad, convert to nanoFarads (10^-9 Farad).
    // This is  a workaround because Serial.print will not print floats

//    nanoFarads = microFarads * 1000.0;      // multiply by 1000 to convert to nanoFarads (10^-9 Farads)
//    Serial.print((float)nanoFarads);         // print the value to serial port
//    Serial.println(" nanoFarads");          // print units and carriage return
//  }

  /* dicharge the capacitor  */
  digitalWrite(chargePin13, LOW);             // set charge pin to  LOW
  digitalWrite(chargePin12, LOW);             // set charge pin to  LOW
  pinMode(dischargePin11, OUTPUT);            // set discharge pin to output
  digitalWrite(dischargePin11, LOW);          // set discharge pin LOW
  while(analogRead(analogPin) > 0){         // wait until capacitor is completely discharged
  }

  pinMode(dischargePin11, INPUT);            // set discharge pin back to input

  delay(100);
}
