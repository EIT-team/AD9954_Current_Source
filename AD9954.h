/* 
   AD9954.h - AD9954 DDS communication library
   Created by Neal Pisenti, 2013.
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
   aunsigned long with this program.  If not, see <http://www.gnu.org/licenses/>.


 */

#ifndef AD9954_h
#define AD9954_h

#include "Arduino.h"

class AD9954
{
    public: 
        // Constructor function. 
        AD9954( byte, byte, byte, byte, byte, byte);

        // Initialize with refClk frequency
        void initialize(unsigned long);

        // Initialize with refIn frequency, and clock multiplier value
        void initialize(unsigned long, byte);

        // Reset the DDS
        void reset();

        // Update the new frequency tuning word
        void update();
        

        // Gets current frequency
        unsigned long getFreq();
        // Sets frequency
        void setFreq(unsigned long);

        // Gets current frequency tuning word
        unsigned long getFTW();
        // Sets frequency tuning word
        void setFTW(unsigned long);

        // places DDS in linear sweep mode
        void linearSweep(unsigned long, unsigned long, unsigned long, byte, unsigned long, byte);



        // DDS frequency resolution
         static constexpr double RESOLUTION = 4294967296; // sets resolution to 2^32 = 32 bits. Using type double to avoid confusion with integer division...


    private:
        // Instance variables that hold pinout mapping
        // from arduino to DDS pins.
        byte _ssPin, _resetPin, _updatePin, _ps0, _ps1, _osk;

        // Instance variables for frequency _freq, frequency tuning word _ftw,
        // reference clock frequency _refClk, etc.
        unsigned long _freq, _ftw, _refClk, _refIn;


        // function to write data to register. 
        void writeRegister(byte[2], byte[1024]);





};
 

#endif
