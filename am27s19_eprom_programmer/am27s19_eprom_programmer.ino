/*
 * Bipolar EPROM controller for AM27S19 devices
 * 
 * Pinout: 13 = LED (lit when programming)
 *          1 = program button (active Low)
 *          2 = CS
 *          3 = CSP
 *          4 = PROG
 *          
 * External circuitry required:
 * * Address select switches
 * * Data switches (these have to be wired to deliver the programming pulse when closed and pulled down otherwise)
 * * Programming voltage select circuitry
 * * CS_ control circuitry, making CS_ able to take three distinct voltages:
 *    CS_ =  0 V : EPROM is in read only mode and is reading out data (CS=HIGH, CSP=LOW)
 *    CS_ =  5 V : EPROM is in read only mode but disabled (CS=LOW, CSP=LOW)
 *    CS_ = 15 V : EPROM should commence programming  (CS=LOW, CSP=HIGH)
 *   NB: CS=HIGH,CSP=HIGH is an undefined state and should be avoided!
 *          
 * Theory of operation:
 * When the CS pin is HIGH, the programming function is disabled; the EPROM is read only and produces reads data at the specified address.
 * When CSP is HIGH, the CS_ pin on the EPROM is raised to 15V
 * When PROG is HIGH, 20V is applied to the bit being programmed
 * 
 */

#define LED 13
#define BUTTON 1
#define CS   2
#define CSP 3
#define PROG 4

void setup() {
  // put your setup code here, to run once:

  pinMode(LED,OUTPUT);
  pinMode(BUTTON,INPUT);
  pinMode(CS,OUTPUT);
  pinMode(CSP,OUTPUT);
  pinMode(PROG,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Idle state
  digitalWrite(CS,HIGH);
  digitalWrite(CSP,LOW);
  digitalWrite(PROG,LOW);
  digitalWrite(LED,HIGH);

  if( digitalRead(BUTTON) == 0 ){
    // Begin programming cycle

    digitalWrite(CS,LOW);
    digitalWrite(CSP,LOW);
    digitalWrite(PROG,LOW);
    digitalWrite(LED,LOW);

    delayMicroseconds(1);

    digitalWrite(PROG,HIGH);

    delayMicroseconds(1);

    digitalWrite(CSP,HIGH);

    delayMicroseconds(60); // This is the actual time when the bits are burned

    digitalWrite(PROG,LOW);

    delayMicroseconds(1);
    digitalWrite(CSP,LOW);

    delayMicroseconds(3);

    digitalWrite(CS,HIGH); // Forcably discharge any parasitic capacitance on CS_
    digitalWrite(PROG,LOW);

    delay(100); // Debouncing lockout
    digitalWrite(LED,HIGH);
  }
}
