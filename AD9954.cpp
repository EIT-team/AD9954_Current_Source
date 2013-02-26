/*
   AD9954.cpp - AD9954 DDS communication library
   Created by Neal Pisenti, 2013
   JQI - Strontium - UMD

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */


#include "Arduino.h"
#include "SPI.h"
#include <AD9954.h>


/* CONSTRUCTOR */

// Constructor function; initializes communication pinouts
AD9954::AD9954(byte ssPin, byte resetPin, byte updatePin, byte ps0, byte ps1, byte osk)
{
    _ssPin = ssPin;
    _resetPin = resetPin;
    _updatePin = updatePin;
    _ps0 = ps0;
    _ps1 = ps1;
    _osk = osk;


    // sets up the pinmodes for output
    pinMode(_ssPin, OUTPUT);
    pinMode(_resetPin, OUTPUT);
    pinMode(_updatePin, OUTPUT);
    pinMode(_ps0, OUTPUT);
    pinMode(_ps1, OUTPUT);
    pinMode(_osk, OUTPUT);

    // defaults for pin logic levels
    digitalWrite(_ssPin, HIGH);
    digitalWrite(_resetPin, LOW);
    digitalWrite(_updatePin, LOW);
    digitalWrite(_ps0, LOW);
    digitalWrite(_ps1, LOW);
    digitalWrite(_osk, LOW);

    
}


/* PUBLIC CLASS FUNCTIONS */


// initialize(refClk) - initializes DDS with reference clock frequency refClk
void AD9954::initialize(unsigned long refClk){
    Serial.println("aaa");
    _refIn = refClk;
    _refClk = refClk;
    AD9954::reset();
     
}

// initialize(refClk, clkMult) -- initializes DDS with input refClk, and activates the
//      onboard PLL multiplier clkMult.
//      clkMult: must be integer between 4 and 20 inclusive.
void AD9954::initialize(unsigned long refClk, byte clkMult){
    _refIn = refClk;
    _refClk = _refIn * clkMult;

    // resets DDS
    AD9954::reset();

    byte multValue = clkMult;

    byte registerInfo[] = {0x01, 3};
    byte data[] = {0x00, 0x00, 0x00};

    // writes value for clock multiplier
    if (_refClk < 25000000){ 
        multValue <<= 1;
        //multValue = multValue << 1;
    }else{
        (multValue <<= 1)++;
        //multValue = (multValue << 1) + 1;
    }

    multValue <<= 2;
    //multValue = multValue << 2;

    data[2] = lowByte(multValue);

    AD9954::writeRegister(registerInfo, data);
}
        



// reset() - takes no arguments; resets DDS
void AD9954::reset(){
    digitalWrite(_resetPin, HIGH);
    delay(1);
    digitalWrite(_resetPin, LOW);
}

// update() - sends a logic pulse to IO UPDATE pin on DDS; updates frequency output to 
//      newly set frequency (FTW0)
void AD9954::update(){
    digitalWrite(_updatePin, HIGH);
    digitalWrite(_updatePin, LOW);
}


// setFreq(freq) -- writes freq to DDS board, in FTW0
void AD9954::setFreq(unsigned long freq){
    // set _freq and _ftw variables
    _freq = freq;
    _ftw = freq * RESOLUTION / _refClk ;

    // divide up ftw into four bytes
    byte ftw[] = { lowByte(_ftw >> 24), lowByte(_ftw >> 16), lowByte(_ftw >> 8), lowByte(_ftw)};
    // register info -- writing four bytes to register 0x04, 
    byte registerInfo[] = {0x04, 4};

    // actually writes to register
    AD9954::writeRegister(registerInfo, ftw);
    // issues update command
    AD9954::update();
}


// getFreq() - returns current frequency
unsigned long AD9954::getFreq(){
    return _freq;
}

// getFTW() -- returns current FTW
unsigned long AD9954::getFTW(){
    return _ftw;
}

// Function setFTW -- accepts 32-bit frequency tuning word ftw;
//      updates instance variables for FTW and Frequency, and writes ftw to DDS.
void AD9954::setFTW(unsigned long ftw){
    // set freqency and ftw variables
    _ftw = ftw;
    _freq = ftw * _refClk / RESOLUTION;

    // divide up ftw into four bytes
    byte data[] = { lowByte(_ftw >> 24), lowByte(_ftw >> 16), lowByte(_ftw >> 8), lowByte(_ftw)};
    // register info -- writing four bytes to register 0x04, 
    byte registerInfo[] = {0x04, 4};

    AD9954::writeRegister(registerInfo, data);
    AD9954::update();

}




/* PRIVATE CLASS FUNCTIONS */


// Writes SPI to particular register.
//      registerInfo is a 2-element array which contains [register, number of bytes]
void AD9954::writeRegister(byte registerInfo[], byte data[]){

    digitalWrite(_ssPin, LOW);

    // Writes the register value
    SPI.transfer(registerInfo[0]);

    // Writes the data
    for(int i = 0; i < registerInfo[1]; i++){
        SPI.transfer(data[i]);
    }

    digitalWrite(_ssPin, HIGH);

}

