#ifndef _SETTINGS_H
#define _SETTINGS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// Measurement settings

const int CS_Freq = 1000; // Carrier frequency
const int injTime = 1000; // Injection time in ms

const int nprt = 15;
const int Protocol[nprt][2] = { {1, 2}, {2, 3}, {3, 4}, {4, 5},{5, 6},{6, 7},{7, 8},{9, 10},{10, 11},{11, 12},{12, 13},{13, 14},{14, 15},{15, 16},{16, 1} };




// Hardware setup

const int NumBoard = 1;
const int TotalPins = 40 * NumBoard;
const int chnmax = 199; // maximum number of channels

#endif