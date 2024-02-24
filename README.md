# AM27S19_eprom_programmer
Manual programmer for AM27S19 bipolar EPROM devices

## Synopsis

This is simple hardware programmer for AM27S19 bipolar EPROM devices.  These are TTL logic devices that have 5 bit address and 8 bit data, for a total of (32 x 8 = 256) bits of storage.  The programmer is manually operated: the user selects the desired address using 5 DIP switches, and then can read or write the data at that address.  Each data bit is programmed individually by means of 8 DIP switches.

When blank, AM27S19 bipolar EPROM devices have all 0 (LOW) data.  Programming a bit causes it to be permanently a 1 (HIGH) data value.  The programming process is *irreversible*.  Once a bit is a 1 (HIGH), it cannot be changed!

There is no LED display of the data, mostly because the programming pulses are sufficiently high voltage that they can damage LEDs.  I didn't want to add protective circuitry.  To verify the contents on the EPROM, simply use a logic probe to read individual data bit positions at the test points near the DATA switches.

## Usage instructions

The programmer uses a single +20 V DC power input and generates the other needed voltages from linear regulators.  (This is not efficient but it is sufficient.  The regulators do tend to run hot, but this is not usually a problem.)

1. With power disconnected, set all DATA switches to OPEN, and install the blank AM27S19 EPROM into its socket.
2. Apply +20 V DC power.
3. Reading:
 a. Select the desired address on the ADDRESS switches
 b. OPEN all DATA switches
 c. Using a logic probe, powered from the +5 V DC regulated output, measure the test points at each of the DATA switches.
 d. Repeat (a)-(b) for any other DATA bit positions and ADDRESSes as desired.
4. Writing:
 a. Select the desired address on the ADDRESS switches
 b. OPEN all DATA switches
 c. Bits are programmed individually.  If the bit in question is desired to be 0 (LOW), do not do anything!
 d. If the bit in question is desired to be a 1 (HIGH), CLOSE the *one* DATA switch corresponding to the bit.
 e. Disconnect the logic probe if it was connected.
 f. Briefly press the PROGRAM switch exactly one time.  (Multiple presses should be unnecessary and can cause the EPROM to overheat.)
 g. OPEN the DATA switch
 h. Follow step (3) to verify that the bit was correctly programmed to a 1 (HIGH)
 i. Repeat (a)-(h) for all other bits.
5. Disconnect power before removing the programmed EPROM.

## Theory of operation

When the EPROM is being read, the CS_ line is held LOW.  This is the usual state of the programmer circuit.  ADDRESS switches are rapidly decoded into DATA.

There are two signals used to perform the programming process: CS_ and the data line in question.  "Programming" means specifically the process of permanently changing a bit from a 0 (LOW) to a 1 (HIGH).

### Timing

The signal timing and overall control is handled by a Seeed Studio XIAO SAMD21 microcontroller.  The signal timing requirements are detailed in the included PDF datasheet, extracted from an old AMD databook.  See attached [programming.png], showing oscilloscope traces of my programmer successfully programming a bit from a 0 (LOW) to a 1 (HIGH).

After the user presses the PROGRAM button, the process is as follows, with short delays (around a microsecond) between each step:
1. The CS_ line is changed to a TTL HIGH, which should be above +2.5 V and below +5.5 V.
2. The data line in question is raised to +20 V.
3. The CS_ line is raised to +15 V.
4. The signals are held constant for around 50-75 microseconds
5. The CS_ line is returned to a TTL HIGH, and the data line is brought to LOW.
6. The signals are held constant for around 100 microseconds or more to prevent excessive heating.
7. The CS_ line is brought to LOW to return the EPROM to a read state.

### Voltage level control

The CS_ line is controlled by several transistor switches to select one of three possible voltages: 0 V, 5 V, and 15 V.  These are selected using two control lines from the microcontroller (CS, CSP):

* CS_ =  0 V : EPROM is in read only mode and is reading out data (CS=HIGH, CSP=LOW)
* CS_ =  5 V : EPROM is in read only mode but disabled (CS=LOW, CSP=LOW)
* CS_ = 15 V : EPROM should commence programming  (CS=LOW, CSP=HIGH)
* NB: CS=HIGH,CSP=HIGH is an undefined state and should be avoided!  (Current limiting resistors are included in the circuit to avoid damaging the transistors should this state be entered accidentally.)

The data line to be programmed is selected using the DATA switches, and is driven by a transistor switch that selects between 0 V and 20 V.