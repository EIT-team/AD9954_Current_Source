# AD9954 communication library

## Overview

This library facilitates communication between the Arduino and Analog Devices AD9954 DDS chip.

## Getting Started

You'll first need to include the SPI library, as well as this one. Then, you can declare an AD9954 object, like

    #include <SPI.h>
    #include <AD9954.h>
    AD9954 DDS(ssPin, resetPin, updatePin, ps0, ps1, osk);

In your `void setup(){ ... }` function, you'll need to first start the SPI bus:

    SPI.begin();
    SPI.setClockDivider(4);
    SPI.setDataMode(SPI_MODE0);
    delay(500); // let things get set up...

(note, this is assuming you're using an Arduino Due...)

Then, initialize your DDS with a clock frequency:

    DDS.initialize(400000000); // initialize DDS with 400 MHz clock

## Currently implemented Features

I've currently implemented the basic single-tone mode, and the linear sweep mode.

### Single-tone mode

After you've initialized the DDS, you can command it to output a particular frequency like

    DDS.setFreq(25000000); // set DDS to 25 MHz

### Linear sweep mode

You can also do a bi-directional linear sweep, using `DDS.linearSweep()`. The direction of the ramp is controlled by 
the `PS0` pin, so `PS0 HIGH` will cause it to do a positive ramp (limited by a final frequency), and `PS0 LOW` will make it 
ramp down (limited by a lower-bound frequency). Eg,

    
    // Function linearSweep -- places DDS in linear sweep mode.
    //      Behavior is determined by two frequency tuning words, freq0 and freq1 (freq0 < freq1).
    //      The PS0 pin HIGH will ramp towards freq1, PS0 LOW will ramp towards freq0.
    //      The rate of the ramp is dictated by pos/negDF (positive/negative DeltaFreq), and pos/negRR (positive/negative RampRate).
    //      
    //      freq0: lower frequency bound (Hz)
    //      freq1: upper frequency bound (Hz)
    //      posDF: delta frequency for positive ramp (Hz)
    //      negDF: delta frequency for negative ramp (Hz)
    //      posRR: number between 0 and 255, indicating number of SYNC_CLK cycles spent at each
    //              frequency value in the ramp. SYNC_CLK operates at 1/4 of the SYSCLK clock value. Typically SYNC_CLK = 100MHz.
    //              Thus, the true "ramp rate" is, eg, posDF/(posRR*10 ns)
    //      negRR: same as above, but for negative ramp.
    //      
    //      As a general rule, round up (not down) in calculating the delta frequency steps.
    //
    //      AD9954::linearSweep(freq0, freq1, posDF, posRR, negDF, negRR);

    DDS.linearSweep(19500000, 20500000, 1, 1, 1, 1);

This would set the DDS for a linear sweep between 19.5 and 20.5 MHz, where the ramp rate is set at one SYNC\_CLK cycle per frequency
step (pos/negRR = 1), and 1 Hz between each frequency in the ramp (pos/negDF = 1). Thus, for a 400MHz clock, SYNC\_CLK is 100 MHz 
(10ns per cycle) and so the ramp rate is

    1 Hz/10 ns = 100 Hz per microsecond

Toggling the `PS0` pin instructs the DDS which way to ramp; if it reaches the frequency bounds, it will hold that frequency until `PS0`
is toggled in the opposite direction.

    
    
