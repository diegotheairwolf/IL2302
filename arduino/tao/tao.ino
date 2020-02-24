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

//#define analogPin      0            // analog pin for measuring capacitor voltage
#define chargePin13      13         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin11   11         // pin to discharge the capacitor
#define chargePin12      12         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin10   10         // pin to discharge the capacitor
#define A              4            // pin to select multiplexer channel. LSB
#define B              3            // pin to select multiplexer channel.
#define C              2            // pin to select multiplexer channel. MSB
#define resistorValue  1000000.0F   // change this to whatever resistor value you are using
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
int  count = 0;
int analogPin = 0;

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
  /* select channel */
  selChannel(count);
  
  /* charge the capacitor*/
  selChargePin(count);
  
  startTime = micros();

  while(analogRead(analogPin) < 648){       // 647 is 63.2% of 1023, which corresponds to full-scale voltage
  }

  elapsedTime= micros() - startTime;
  
  // convert microseconds to seconds ( 10^-6 ) and Farads to picoFarads ( 10^12 ),  net 10^6 (1000000)
  microFarads = ((float)elapsedTime / resistorValue) * 1000000.0;
  Serial.print(" cap ");            // print the value to serial port
  Serial.print(count);              // print the value to serial port
  Serial.print(" ");          // print units and carriage return
  Serial.print(elapsedTime);        // print the value to serial port
  Serial.print(" mS    ");          // print units and carriage return
  Serial.print((float)microFarads,5);     // print the value to serial port
  Serial.println(" picoFarads");         // print units and carriage return

  
  /* dicharge the capacitor  */
  digitalWrite(chargePin13, LOW);             // set charge pin to  LOW
  digitalWrite(chargePin12, LOW);             // set charge pin to  LOW
  
  pinMode(dischargePin11, OUTPUT);            // set discharge pin to output
  pinMode(dischargePin10, OUTPUT);            // set discharge pin to output
  
  digitalWrite(dischargePin11, LOW);          // set discharge pin LOW
  digitalWrite(dischargePin10, LOW);          // set discharge pin LOW
  
  while(analogRead(analogPin) > 0){         // wait until capacitor is completely discharged
  }

  pinMode(dischargePin11, INPUT);            // set discharge pin back to input
  pinMode(dischargePin10, INPUT);            // set discharge pin back to input


  count++;
  count = count%2;

  delay(500);
}

void selChargePin(int count){
  if(count==1){
      digitalWrite(chargePin13, LOW);   // set charegePin13 LOW and capacitor charging
      digitalWrite(chargePin12, HIGH);  // set charegePin12 HIGH and capacitor charging
      analogPin = 1;
  }
  else {
      digitalWrite(chargePin13, HIGH);  // set charegePin13 HIGH and capacitor charging
      digitalWrite(chargePin12, LOW);   // set charegePin12 LOW and capacitor charging
      analogPin = 0;
  }
  
  
}

void selChannel(int count){
  switch(count){
    case 0:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      break;
    case 1:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW); 
      break;
    case 2:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW); 
      break;
    case 3:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW); 
      break;
    case 4:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH); 
      break;
    case 5:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH); 
      break;
    case 6:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH); 
      break;
    case 7:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH); 
      break;
    default:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW); 
      break;
      
  }
  
}
